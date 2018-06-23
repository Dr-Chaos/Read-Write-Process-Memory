# Read-Write-Process-Memory

An easy to use header, for read and write process memory ! 
Useful for hacking, create game trainers, game bots, etc


## How to use
1. Download and add memorymanager.h to your project
2. Add use the library like this

```
#include "memorymanager.h"

// ...

int main()
{
	memory_manager memory;
	
	// open process
	memory.window("Breakpoint World Tennis Tournament");
	
	// read and write int address value
	auto address_int = 0x7FF763ADBE4C;
	cout << "Address value: " << memory.read(address_int) << endl;
	memory.write_int(address_int, 50);

	// read and write string address value
	auto address_string = 0x9LL193JQWE1P;
	cout << "Actual address_string value: " << memory.read_string(address_string) << endl;
	memory.write_string(address_string, "Hello");
	
	// advanced features for module base address
	const auto module_name = "wttpc.exe";
	const auto offset_address_int = 0x8B6BE4C;
	cout << "read_base_int value: " << memory.read_base_int(module_name, offset_address_int) << endl;
	memory.write_base_int(module_name, offset_address_int, 133);

	const auto offset_address_string = 0x8434978;
	cout << "read_base_string value: " << memory.read_base_string(module_name, offset_address_string) << endl;
	memory.write_base_string(module_name, offset_address_string, "Yattaaa");
}
```


Note: see main.cpp for exemples
