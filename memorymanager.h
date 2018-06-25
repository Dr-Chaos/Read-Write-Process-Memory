#pragma once

/************************************************************************
* Author: Dr-Chaos														*
* Official repo: https://github.com/Dr-Chaos/Read-Write-Process-Memory	*
*************************************************************************/

#include <Windows.h>
#include <iostream>
#include <string>
#include <tchar.h>
#include <TlHelp32.h>


class memory_manager
{
public:
	// load process by window name
	void window(const char* window_name);

	// read static_address
	int read_int(long long static_address) const;
	float read_float(long long static_address) const;
	std::string read_string(long long static_address) const;


	// write in static_address
	void write_int(long long static_address, int new_data) const;
	void write_float(long long static_address, float new_data) const;
	void write_string(long long static_address, const std::string& new_data) const;

	// get module_base_address
	long long get_module_base_address(const TCHAR* module_name) const;

	// read module_base_address + offset_address = static_address
	int read_base_int(const TCHAR* module_name, long long offset_address) const;
	std::string read_base_string(const TCHAR* module_name, long long offset_address) const;

	// write in module_base_address + offset_address = static_address
	void write_base_int(const TCHAR* module_name, long long offset_address, int new_data) const;
	void write_base_string(const TCHAR* module_name, long long offset_address, const std::string& new_data) const;

	// multi-level pointer: return final pointer address from offets
	DWORD get_address_from_multi_level_pointer(DWORD static_base_address, const DWORD offsets_array[], int nomber_offets) const;

	// global objects
	HANDLE application_process = nullptr;
	DWORD process_pid = 0;
};


/**
* \brief
* \param window_name
*/
inline void memory_manager::window(const char* window_name)
{
	// find the application by window name
	const auto application_window_name = FindWindowA(nullptr, window_name);
	if (application_window_name)
	{
		std::cout << "Success to find the application by window name" << std::endl;

		// gets the process pid
		// DWORD process_pid;
		const auto application_pid = GetWindowThreadProcessId(application_window_name, &process_pid);
		if (application_pid)
		{
			std::cout << "Success to find application process_pid" << std::endl;

			// open the process
			application_process = OpenProcess(PROCESS_ALL_ACCESS, false, process_pid);
			if (application_process)
			{
				std::cout << "Success to open the process" << std::endl;
			}
			else
			{
				std::cout << "Error to open the process" << std::endl;
			}
		}
		else
		{
			std::cout << "Error to find application process_pid" << std::endl;
		}
	}
	else
	{
		std::cout << "Error to find the application by window name" << std::endl;
	}
}

/**
* \brief
* \param static_address
* \return
*/
inline int memory_manager::read_int(const long long static_address) const
{
	int address_value;
	if (ReadProcessMemory(application_process, LPCVOID(static_address), &address_value, sizeof address_value, nullptr))
	{
		// nothind to do
	}
	else
	{
		// convert address_value to string and write error message
		std::to_string(address_value) = "Error read_int";
	}

	return address_value;
}

/**
* \brief
* \param static_address
* \return
*/
inline float memory_manager::read_float(const long long static_address) const
{
	float address_value;
	if (ReadProcessMemory(application_process, LPCVOID(static_address), &address_value, sizeof address_value, nullptr))
	{
		// nothind to do
	}
	else
	{
		// convert address_value to string and write error message
		std::to_string(address_value) = "Error read_float";
	}

	return address_value;
}

/**
* \brief
* \param static_address
* \return
*/
inline std::string memory_manager::read_string(const long long static_address) const
{
	char address_value[256];
	std::string address_value_string;
	if (ReadProcessMemory(application_process, LPVOID(static_address), &address_value, sizeof address_value, nullptr))
	{
		address_value_string = std::string(address_value);
	}
	else
	{
		address_value_string = "Error read_string";
	}

	return address_value_string;
}

/**
* \brief
* \param static_address
* \param new_data
*/
inline void memory_manager::write_int(const long long static_address, int new_data) const
{
	if (WriteProcessMemory(application_process, LPVOID(static_address), &new_data, sizeof new_data, nullptr))
	{
		std::cout << "Success write_int, new value is: " << new_data << std::endl;
	}
	else
	{
		std::cout << "Error write_int" << std::endl;
	}
}

inline void memory_manager::write_float(const long long static_address, float new_data) const
{
	if (WriteProcessMemory(application_process, LPVOID(static_address), &new_data, sizeof new_data, nullptr))
	{
		std::cout << "Success write_float, new value is: " << new_data << std::endl;
	}
	else
	{
		std::cout << "Error write_float" << std::endl;
	}
}

/**
* \brief
* \param static_address
* \param new_data
*/
inline void memory_manager::write_string(const long long static_address, const std::string& new_data) const
{
	if (WriteProcessMemory(application_process, LPVOID(static_address), new_data.c_str(), new_data.length() + 1,
	                       nullptr))
	{
		std::cout << "Success write_string, new value is: " << new_data << std::endl;
	}
	else
	{
		std::cout << "Error write_string" << std::endl;
	}
}

/*
* use processus name for return module base address
* in Cheat Engine: double click on static address, you need to see: wttpc.exe+84391C4
* wttpc.exe is the processus name and 84391C4 is the offset.
* so the module_base_address + offset_address = static_address
*
* !! important !! for use TCHAR*, you need to use:
* auto module_name = _T("module.exe");
*/
inline long long memory_manager::get_module_base_address(const TCHAR* module_name) const
{
	long long module_base_address = 0;
	const auto snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, process_pid);
	if (snapshot != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32 module_entry32;
		module_entry32.dwSize = sizeof(MODULEENTRY32);
		if (Module32First(snapshot, &module_entry32))
		{
			do
			{
				if (_tcsicmp(module_entry32.szModule, module_name) == 0)
				{
					module_base_address = uintptr_t(module_entry32.modBaseAddr);
					break;
				}
			}
			while (Module32Next(snapshot, &module_entry32));
		}
		CloseHandle(snapshot);
	}

	return module_base_address;
}

/**
* \brief
* \param module_name
* \param offset_address
* \return
*/
inline int memory_manager::read_base_int(const TCHAR* module_name, const long long offset_address) const
{
	int address_value;
	const auto module_base_address = get_module_base_address(module_name);
	if (ReadProcessMemory(application_process, LPCVOID(module_base_address + offset_address), &address_value,
	                      sizeof address_value, nullptr))
	{
		// nothind to do
	}
	else
	{
		// convert address_value to string and write error message
		std::to_string(address_value) = "Error read_int";
	}

	return address_value;
}

/**
* \brief
* \param module_name
* \param offset_address
* \return
*/
inline std::string memory_manager::read_base_string(const TCHAR* module_name, const long long offset_address) const
{
	char address_value[256];
	std::string address_value_string;
	const auto module_base_address = get_module_base_address(module_name);
	if (ReadProcessMemory(application_process, LPVOID(module_base_address + offset_address), &address_value,
	                      sizeof address_value, nullptr))
	{
		address_value_string = std::string(address_value);
	}
	else
	{
		address_value_string = "Error read_string";
	}

	return address_value_string;
}

/**
* \brief
* \param module_name
* \param offset_address
* \param new_data
*/
inline void memory_manager::write_base_int(const TCHAR* module_name, const long long offset_address, int new_data) const
{
	const auto module_base_address = get_module_base_address(module_name);
	if (WriteProcessMemory(application_process, LPVOID(module_base_address + offset_address), &new_data,
	                       sizeof new_data, nullptr))
	{
		std::cout << "Success write_base_int, new value is: " << new_data << std::endl;
	}
	else
	{
		std::cout << "Error write_base_int" << std::endl;
	}
}

/**
* \brief
* \param module_name
* \param offset_address
* \param new_data
*/
inline void memory_manager::write_base_string(const TCHAR* module_name, const long long offset_address, const std::string& new_data) const
{
	const auto module_base_address = get_module_base_address(module_name);
	if (WriteProcessMemory(application_process, LPVOID(module_base_address + offset_address), new_data.c_str(),
	                       new_data.length() + 1, nullptr))
	{
		std::cout << "Success write_string, new value is: " << new_data << std::endl;
	}
	else
	{
		std::cout << "Error write_string" << std::endl;
	}
}


/**
 * \brief 
 * \param static_base_address 
 * \param offsets_array 
 * \param nomber_offets 
 * \return 
 */
inline DWORD memory_manager::get_address_from_multi_level_pointer(const DWORD static_base_address, const DWORD offsets_array[], const int nomber_offets) const
{
	DWORD temporary_pointer;
	DWORD pointer_address = 0;

	for (auto i = 0; i < nomber_offets; i++)
	{
		if (i == 0)
		{
			ReadProcessMemory(application_process, LPCVOID(static_base_address), &temporary_pointer, sizeof temporary_pointer, nullptr);
		}
		pointer_address = temporary_pointer + offsets_array[i];
		ReadProcessMemory(application_process, LPCVOID(pointer_address), &temporary_pointer, sizeof temporary_pointer, nullptr);
	}
	return pointer_address;
}
