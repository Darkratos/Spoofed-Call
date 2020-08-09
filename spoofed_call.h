#pragma once

enum spoof_method
{
	inject,
	existing
};

class spoofed_call
{
public:
	/*
	Main Constructor

	Params:
			method		-> determines whether you want to provide an address or inject shellcode
			call_esi	-> optional parameter, use only when you specify the method as 'existing'
	*/
	spoofed_call( spoof_method method, uintptr_t call_esi = 0 );

	/*
	Calls a __cdecl ( WINAPIV ) function with the given parameters

	Params:
			address		-> the target function
			param_count	-> the number of parameters to be parsed to the function
			...			-> your arguments to the function
	*/
	int cdecl_fn( uintptr_t address, int param_count, ... );

	/*
	Calls a __stdcall ( WINAPI ) function with the given parameters

	Params:
			address		-> the target function
			param_count	-> the number of parameters to be parsed to the function
			...			-> your arguments to the function
	*/
	int stdcall_fn( uintptr_t address, int param_count, ... );

	/*
	Calls a __thiscall function with the given parameters

	Params:
			address		-> the target function
			ecx_value	-> self-explanatory
			param_count	-> the number of parameters to be parsed to the function ( excluding the ecx )
			...			-> your arguments to the function
	*/
	int thiscall_fn( uintptr_t address, uintptr_t ecx_value, int param_count, ... );

	/*
	Calls a __fastcall function with the given parameters

	Params:
			address		-> the target function
			ecx_value	-> self-explanatory
			edx_value	-> self-explanatory
			param_count	-> the number of parameters to be parsed to the function ( excluding the ecx and edx )
			...			-> your arguments to the function
	*/
	int fastcall_fn( uintptr_t address, uintptr_t ecx_value, uintptr_t edx_value, int param_count, ... );
private:
	// Local variable to store the call address
	uintptr_t call_esi = 0;
};