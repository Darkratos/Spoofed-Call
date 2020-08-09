#include <Windows.h>
#include "spoofed_call.h"

spoofed_call::spoofed_call( spoof_method method, uintptr_t call_esi )
{
	this->call_esi = call_esi;

	if ( method == spoof_method::inject )
	{
		// TODO
	}
}

__declspec( naked ) int spoofed_call::cdecl_fn( uintptr_t address, int param_count, ... )
{
	__asm
	{	
		push ebp
		mov ebp, esp							
		xor dword ptr[ ebp + 4 ], 0xDEADBEEF	// Spoof the current return with a simple xor
		push esi								// Save some registers
		push edi
		push ecx

		lea esi, param_count					// Put the address of param_count on esi
		mov ecx, [ esi ]						// Save param_count on ecx
		mov edi, ecx							// Save it also in edi, so we can clean up the stack after
		test ecx, ecx							// Check if there are parameters to be pushed
		je after								// There aren't, so we skip the pushes

	push_params:	
		mov eax, [ esi + ecx * 4 ]				// Get the params in the reverse order
		push eax								// And push them in the new stack
		loop push_params						// Loop back until all params are pushed

	after:
		mov eax, [ ebp + 8 ]					// Get the fake return address from the stack
		push [ eax ]							// Push the fake return address
		mov esi, local_return					// Put the local_return on esi
		jmp [ address ]							// Jump to the function

	local_return:
		inc edi									// Increase one so we include the new return address
		imul edi, 4								// Multiply by 4 to get the size to be cleaned
		add esp, edi							// Clean up the stack

		pop ecx									// Restore the saved registers
		pop edi
		pop esi
		xor dword ptr[ ebp + 4 ], 0xDEADBEEF	// Restore the current return
		mov esp, ebp
		pop ebp
		retn
	}
}

__declspec( naked ) int spoofed_call::stdcall_fn( uintptr_t address, int param_count, ... )
{
	__asm
	{
		push ebp
		mov ebp, esp
		xor dword ptr[ ebp + 4 ], 0xDEADBEEF	// Spoof the current return with a simple xor
		push esi								// Save some registers
		push edi
		push ecx

		lea esi, param_count					// Put the address of param_count on esi
		mov ecx, [ esi ]						// Save param_count on ecx
		mov edi, ecx							// Save it also in edi, so we can clean up the stack after
		test ecx, ecx							// Check if there are parameters to be pushed
		je after								// There aren't, so we skip the pushes

	push_params:
		mov eax, [ esi + ecx * 4 ]				// Get the params in the reverse order
		push eax								// And push them in the new stack
		loop push_params						// Loop back until all params are pushed

	after:
		mov eax, [ ebp + 8 ]					// Get the fake return address from the stack
		push [ eax ]							// Push the fake return address
		mov esi, local_return					// Put the local_return on esi
		jmp [ address ]							// Jump to the function

	local_return:
		add esp, 4								// Clean up the stack
		pop ecx									// Restore the saved registers
		pop edi
		pop esi
		xor dword ptr[ ebp + 4 ], 0xDEADBEEF	// Restore the current return
		mov esp, ebp
		pop ebp
		retn
	}
}

__declspec( naked ) int spoofed_call::thiscall_fn( uintptr_t address, uintptr_t ecx_value, int param_count, ... )
{
	__asm
	{
		push ebp
		mov ebp, esp
		xor dword ptr[ ebp + 4 ], 0xDEADBEEF	// Spoof the current return with a simple xor
		push esi								// Save some registers
		push edi
		push ecx

		lea esi, param_count					// Put the address of param_count on esi
		mov ecx, [ esi ]						// Save param_count on ecx
		mov edi, ecx							// Save it also in edi, so we can clean up the stack after
		test ecx, ecx							// Check if there are parameters to be pushed
		je after								// There aren't, so we skip the pushes

	push_params:
		mov eax, [ esi + ecx * 4 ]				// Get the params in the reverse order
		push eax								// And push them in the new stack
		loop push_params						// Loop back until all params are pushed

	after:
		mov eax, [ ebp + 8 ]					// Get the fake return address from the stack
		push [ eax ]							// Push the fake return address
		mov esi, local_return					// Put the local_return on esi
		mov ecx, [ ebp + 0x10 ]					// Put the ecx value
		jmp [ address ]							// Jump to the function

	local_return:
		add esp, 4								// Clean up the stack
		pop ecx									// Restore the saved registers
		pop edi
		pop esi
		xor dword ptr[ ebp + 4 ], 0xDEADBEEF	// Restore the current return
		mov esp, ebp
		pop ebp
		retn
	}
}

__declspec( naked ) int spoofed_call::fastcall_fn( uintptr_t address, uintptr_t ecx_value, uintptr_t edx_value, int param_count, ... )
{
	__asm
	{
		push ebp
		mov ebp, esp
		xor dword ptr[ ebp + 4 ], 0xDEADBEEF	// Spoof the current return with a simple xor
		push esi								// Save some registers
		push edi
		push ecx
		push edx

		lea esi, param_count					// Put the address of param_count on esi
		mov ecx, [ esi ]						// Save param_count on ecx
		mov edi, ecx							// Save it also in edi, so we can clean up the stack after
		test ecx, ecx							// Check if there are parameters to be pushed
		je after								// There aren't, so we skip the pushes

	push_params:
		mov eax, [ esi + ecx * 4 ]				// Get the params in the reverse order
		push eax								// And push them in the new stack
		loop push_params						// Loop back until all params are pushed

	after:
		mov eax, [ ebp + 8 ]					// Get the fake return address from the stack
		push [ eax ]							// Push the fake return address
		mov esi, local_return					// Put the local_return on esi
		mov ecx, [ ebp + 0x10]					// Put the ecx value
		mov edx, [ ebp + 0x14 ]					// Put the edx value
		jmp [ address ]							// Jump to the function

	local_return:
		add esp, 4								// Clean up the stack
		pop edx									// Restore the saved registers
		pop ecx
		pop edi
		pop esi
		xor dword ptr[ ebp + 4 ], 0xDEADBEEF	// Restore the current return
		mov esp, ebp
		pop ebp
		retn
	}
}
