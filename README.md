# Spoofed-Call
A wrapper class to hide the original calling address of a function

# Usage Example
```cpp
#include <Windows.h>
#include <iostream>

#include "spoofed_call.h"

#pragma section( ".text" )
__declspec( allocate( ".text" ) ) const unsigned char call_esi[ ] = { 0xFF, 0xD6 };

void __stdcall test( int a1, int a2, int a3 )
{
	std::cout << a1 + a2 + a3 << std::endl;
}

int main()
{
	spoofed_call* sc = new spoofed_call( spoof_method::existing, ( uintptr_t ) call_esi );
	sc->stdcall_fn( ( uintptr_t ) test, 3, 1, 2, 3 );
}
```
The code above should output '6'.

# Observation
You will need to provide an address which contains an 'call esi'.
The injection part is still not done.
