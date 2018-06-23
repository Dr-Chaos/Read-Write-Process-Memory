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

	memory.window("Breakpoint World Tennis Tournament");

	const auto address_int = 0x7Ff65477BE4C;
	cout << "address_int value: " << memory.read_int(address_int) << endl;
	memory.write_int(address_int, 50);
	// cout << "Type address_int is: " << typeid(memory.read_int(address_int)).name() << endl;

	const auto address_string = 0x7FF654044938;
	cout << "address_string value: " << memory.read_string(address_string) << endl;
	memory.write_string(address_string, "Hello World");
	// cout << "Type address_string is: " << typeid(memory.read_string(address_string)).name() << endl;


	const auto module_name = _T("wttpc.exe");

	const auto offset_address_int = 0x8B6BE4C;
	cout << "read_base_int value: " << memory.read_base_int(module_name, offset_address_int) << endl;
	memory.write_base_int(module_name, offset_address_int, 133);

	const auto offset_address_string = 0x8434978;
	cout << "read_base_string value: " << memory.read_base_string(module_name, offset_address_string) << endl;
	memory.write_base_string(module_name, offset_address_string, "Yattaaa");


	cout << "Press Enter to continue..." << endl;
	cin.ignore().get();
	return 0;
}
