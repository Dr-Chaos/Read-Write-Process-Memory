# Read-Write-Process-Memory

An easy to use header, for read and write process memory ! 
Useful for hacking, create game trainers, game bots, etc


## How to use
1. Download and add memorymanager.h to your project
2. Add the follow lines to you code:

````
#include "memorymanager.h"
// ...
int main()
{
	memory_manager memory;

	memory.window("Breakpoint World Tennis Tournament");
	
	auto int_address = 0x7FF763ADBE4C;
	cout << "Address value: " << memory.read(int_address) << endl;
	memory.write_int(int_address, 55);
	
	memory.write_string(0x9LL193JQWE1P, "my new string");
}
```

Note: see main.cpp for exemples
