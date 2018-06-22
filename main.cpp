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
	
	auto address_int = 0x7FF763ADBE4C;
	cout << "Address value: " << memory.read(address_int) << endl;
	memory.write_int(address_int, 50);

	auto address_string = 0x9LL193JQWE1P;
	cout << "Actual address_string value: " << memory.read_string(address_string) << endl;
	memory.write_string(address_string, "Hello");

	cout << "Press Enter to continue..." << endl;
	cin.ignore().get();
	return 0;
}
