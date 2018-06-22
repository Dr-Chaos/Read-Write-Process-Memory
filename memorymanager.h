#pragma once


#include <Windows.h>
#include <iostream>
#include <string>


class memory_manager
{
public:
	void window(const char* window_name);
	std::string read(long long address) const;
	void write_int(long long address, int new_data) const;
	void write_string(long long address, const std::string& new_data) const;

private:
	HANDLE application_process = nullptr;
};



inline void memory_manager::window(const char* window_name)
{
	// find the application by window name
	const auto application_window_name = FindWindowA(nullptr, window_name);
	if (application_window_name)
	{
		std::cout << "Success to find the application by window name" << std::endl;

		//Gets the process pid
		DWORD pid;
		const auto application_pid = GetWindowThreadProcessId(application_window_name, &pid);
		if (application_pid)
		{
			std::cout << "Success to find application pid" << std::endl;

			//Open the process
			application_process = OpenProcess(PROCESS_ALL_ACCESS, false, pid);
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


inline std::string memory_manager::read(const long long address) const
{
	DWORD address_value;
	if (ReadProcessMemory(application_process, LPCVOID(address), &address_value, sizeof address_value, nullptr))
	{
		//cout << "Succes read process memory, the value is: " << address_value << std::endl;
	}
	else
	{
		//cout << "Error to read process memory" << std::endl;

		// convert address_value to string
		std::to_string(address_value) = "Error to read process memory";
	}

	return std::to_string(address_value);
}

inline void memory_manager::write_int(const long long address, int new_data) const
{
	if (WriteProcessMemory(application_process, LPVOID(address), &new_data, sizeof new_data, nullptr))
	{
		std::cout << "Success to write process memory, new value is: " << new_data << std::endl;
	}
	else
	{
		std::cout << "Error to write process memory" << std::endl;
	}
}

inline void memory_manager::write_string(const long long address, const std::string& new_data) const
{
	if (WriteProcessMemory(application_process, LPVOID(address), new_data.c_str(), new_data.length() + 1, nullptr))
	{
		std::cout << "Success to write process memory, new value is: " << new_data << std::endl;
	}
	else
	{
		std::cout << "Error to write process memory" << std::endl;
	}
}