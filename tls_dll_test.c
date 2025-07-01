#include "tls_test.h"
#include <Windows.h>

__declspec(thread) Test test;

DWORD dll_thread_entry_point(void *ptr)
{
	printf_func *printf = (printf_func *)ptr;
	test.foo = 7;
	test.bar = 8;
	printf("On thread %d in DLL test = { foo = %d, bar = %d}\n", GetCurrentThreadId(), test.foo, test.bar);
	return 0;
}

EXTERN_C __declspec(dllexport) void tls_dll_entry_point(printf_func *printf)
{
	test.foo = 5;
	test.bar = 6;
	printf("On thread %d in DLL test = { foo = %d, bar = %d}\n", GetCurrentThreadId(), test.foo, test.bar);
	HANDLE thread = CreateThread(NULL, 0, dll_thread_entry_point, (void *)printf, 0, NULL);
	WaitForSingleObject(thread, INFINITE);
	printf("On thread %d in DLL test = { foo = %d, bar = %d}\n", GetCurrentThreadId(), test.foo, test.bar);
}
