#include "memorymanager.h"
#include <iostream>


using namespace std;


int main()
{
	memory_manager memory;

	memory.window("Breakpoint World Tennis Tournament");
	cout << "Address value: " << memory.read(0x7FF763ADBE4C) << endl;
	memory.write_int(0x7FF763ADBE4C, 33);

	cout << "Press Enter to continue..." << endl;
	cin.ignore().get();
	return 0;
}
