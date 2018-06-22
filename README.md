# Read-Write-Process-Memory

An easy to use header, for read and write process memory ! 
Useful for hacking, create game trainers, game bots, etc


## How to use
1. Download and add memorymanager.h to your project
2. Add the follow lines to you code:

```
#include "memorymanager.h"

// ...

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
}
```


Note: see main.cpp for exemples
