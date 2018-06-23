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

	void window(const char* window_name);

	// read static_address
	int read_int(long long static_address) const;
	std::string read_string(long long static_address) const;

	// write static_address
	void write_int(long long static_address, int new_data) const;
	void write_string(long long static_address, const std::string& new_data) const;

	// get module_base_address
	uintptr_t get_module_base_address(const TCHAR* sz_module_name) const;

	// read module_base_address + offset_address = static_address
	int read_base_int(const TCHAR* module_name, long long offset_address) const;
	std::string read_base_string(const TCHAR* module_name, long long offset_address) const;

	// write in module_base_address + offset_address = static_address
	void write_base_int(const TCHAR* module_name, long long offset_address, int new_data) const;
	void write_base_string(const TCHAR* module_name, long long offset_address, const std::string& new_data) const;


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
			std::cout << "Success to find application pid" << std::endl;

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
			std::cout << "Error to find application pid" << std::endl;
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
 * So the module_base_address + offset_address = static_address
 */
inline uintptr_t memory_manager::get_module_base_address(const TCHAR* sz_module_name) const
{
	uintptr_t module_base_address = 0;
	const auto h_snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, process_pid);
	if (h_snapshot != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32 module_entry32;
		module_entry32.dwSize = sizeof(MODULEENTRY32);
		if (Module32First(h_snapshot, &module_entry32))
		{
			do
			{
				if (_tcsicmp(module_entry32.szModule, sz_module_name) == 0)
				{
					module_base_address = uintptr_t(module_entry32.modBaseAddr);
					break;
				}
			}
			while (Module32Next(h_snapshot, &module_entry32));
		}
		CloseHandle(h_snapshot);
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
