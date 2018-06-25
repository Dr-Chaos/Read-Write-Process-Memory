/************************************************************************
* Author: Dr-Chaos														*
* Official repo: https://github.com/Dr-Chaos/Read-Write-Process-Memory	*
*************************************************************************/

#include "memorymanager.h"
#include <iostream>


using namespace std;


int main()
{
	memory_manager memory;

	/******************************
	* open process by window name *
	*******************************/
	memory.window("Breakpoint World Tennis Tournament");

	/**************************
	 * read/write address_int *
	 **************************/
	const auto address_int = 0x7Ff65477BE4C;
	cout << "address_int value: " << memory.read_int(address_int) << endl;
	memory.write_int(address_int, 50);
	// cout << "Type address_int is: " << typeid(memory.read_int(address_int)).name() << endl;

	
	/*****************************
	 * read/write address_string *
	 *****************************/
	const auto address_string = 0x7FF654044938;
	cout << "address_string value: " << memory.read_string(address_string) << endl;
	memory.write_string(address_string, "Hello World");
	// cout << "Type address_string is: " << typeid(memory.read_string(address_string)).name() << endl;


	/**********************************
	 * read/write base_module_address *
	 **********************************/
	const auto module_name = _T("wttpc.exe");

	const auto static_base_address_int = 0x8B6BE4C;
	cout << "read_base_int value: " << memory.read_base_int(module_name, static_base_address_int) << endl;
	memory.write_base_int(module_name, static_base_address_int, 133);

	const auto static_base_address_string = 0x8434978;
	cout << "read_base_string value: " << memory.read_base_string(module_name, static_base_address_string) << endl;
	memory.write_base_string(module_name, static_base_address_string, "Yattaaa");

	/*********************************
	 * red/write multi-lever pointer *
	 *********************************/
	const auto static_base_address = 0x00B12B64;
	DWORD offsets[] = { 0xD0, 0x1B0, 0x2C, 0x18 };
	const auto multi_level_pointer_address = memory.get_address_from_multi_level_pointer(static_base_address, offsets, 4);
	cout << memory.read_float(multi_level_pointer_address) << endl;
	memory.write_float(multi_level_pointer_address, 10);


	cout << "Press Enter to continue..." << endl;
	cin.ignore().get();
	return 0;
}
