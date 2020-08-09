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

int spoofed_call::cdecl_fn( uintptr_t address, int param_count, ... )
{
	uintptr_t call_esi = this->call_esi;

	__asm
	{
		push esp					// Save some registers
		push ebp
		push esi
		push edi
		push ecx

		lea esi, param_count		// Put the address of param_count on esi
		mov ecx, [ esi ]			// Save param_count on ecx
		mov edi, ecx				// Save it also in edi, so we can clean up the stack after
		add esi, 4					// Add 4 so esi has the parameters
		cmp ecx, 0					// Check if it has parameters
		je after					// If it doesn't, skip the pushes

	push_params:
		dec ecx
		mov eax, [ esi + ecx * 4 ]	// Get the params in the reverse order
		push eax					// And push them in the new stack
		cmp ecx, 0
		jne push_params				// Loop back until all params are pushed

	after:
		push call_esi				// Push the fake return address
		mov esi, local_return		// Put the local_return on esi
		jmp [ address ]				// Jump to the function

	local_return:
		add esp, 4					// Clean up the stack after the call
		mov ecx, edi
		imul ecx, 4
		add esp, ecx
		pop ecx						// Restore the saved registers
		pop edi
		pop esi
		pop ebp
		pop esp
	}
}

int spoofed_call::stdcall_fn( uintptr_t address, int param_count, ... )
{
	uintptr_t call_esi = this->call_esi;

	__asm
	{
		push esp					// Save some registers
		push ebp
		push esi
		push ecx

		lea esi, param_count		// Put the address of param_count on esi
		mov ecx, [ esi ]			// Save param_count on ecx
		add esi, 4					// Add 4 so esi has the parameters
		cmp ecx, 0					// Check if it has parameters
		je after					// If it doesn't, skip the pushes

	push_params:
		dec ecx
		mov eax, [ esi + ecx * 4 ]	// Get the params in the reverse order
		push eax					// And push them in the new stack
		cmp ecx, 0
		jne push_params				// Loop back until all params are pushed

	after:
		push call_esi				// Push the fake return address
		mov esi, local_return		// Put the local_return on esi
		jmp [ address ]				// Jump to the function

	local_return:
		add esp, 4					// Clean up the stack after the call
		pop ecx						// Restore the saved registers
		pop esi
		pop ebp
		pop esp
	}
}

int spoofed_call::thiscall_fn( uintptr_t address, uintptr_t ecx_value, int param_count, ... )
{
	uintptr_t call_esi = this->call_esi;

	__asm
	{
		push esp					// Save some registers
		push ebp
		push esi
		push ecx

		lea esi, param_count		// Put the address of param_count on esi
		mov ecx, [ esi ]			// Save param_count on ecx
		add esi, 4					// Add 4 so esi has the parameters
		cmp ecx, 0					// Check if it has parameters
		je after					// If it doesn't, skip the pushes

	push_params:
		dec ecx
		mov eax, [ esi + ecx * 4 ]	// Get the params in the reverse order
		push eax					// And push them in the new stack
		cmp ecx, 0					// Loop back until all params are pushed
		jne push_params

	after:
		push call_esi				// Push the fake return address
		mov esi, local_return		// Put the local_return on esi
		mov ecx, ecx_value			// Parse the 'this' to ecx
		jmp [ address ]				// Jump to the function

	local_return:
		add esp, 4					// Clean up the stack after the call
		pop ecx						// Restore the saved registers
		pop esi
		pop ebp
		pop esp
	}
}

int spoofed_call::fastcall_fn( uintptr_t address, uintptr_t ecx_value, uintptr_t edx_value, int param_count, ... )
{
	uintptr_t call_esi = this->call_esi;

	__asm
	{
		push esp					// Save some registers
		push ebp
		push esi
		push ecx
		push edx

		lea esi, param_count		// Put the address of param_count on esi
		mov ecx, [ esi ]			// Save param_count on ecx
		add esi, 4					// Add 4 so esi has the parameters
		cmp ecx, 0					// Check if it has parameters
		je after					// If it doesn't, skip the pushes

	push_params:
		dec ecx
		mov eax, [ esi + ecx * 4 ]	// Get the params in the reverse order
		push eax					// And push them in the new stack
		cmp ecx, 0					// Loop back until all params are pushed
		jne push_params

	after:
		push call_esi				// Push the fake return address
		mov esi, local_return		// Put the local_return on esi
		mov ecx, ecx_value			// Parse the ecx value
		mov edx, edx_value			// Parse the edx value
		jmp [ address ]				// Jump to the function

	local_return:
		add esp, 4					// Clean up the stack after the call
		pop edx						// Restore the saved registers
		pop ecx
		pop esi
		pop ebp
		pop esp
	}
}