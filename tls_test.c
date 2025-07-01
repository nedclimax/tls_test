#define STB_SPRINTF_IMPLEMENTATION
#define STB_SPRINTF_NOFLOAT
#include "stb_sprintf.h"
#include <stdarg.h>
#include <stdint.h>
#include <Windows.h>

typedef struct Test {
	int64_t foo;
	int64_t bar;
} Test;

__declspec(thread) Test test;

void my_printf(const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	int size = stbsp_vsnprintf(NULL, 0, fmt, args);
	char *buf = (char *)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, size + 1);
	stbsp_vsnprintf(buf, size + 1, fmt, args);
	va_end(args);
	HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);
	WriteConsoleA(hstdout, buf, size + 1, NULL, NULL);
	HeapFree(GetProcessHeap(), 0, buf);
}

DWORD thread_entry_point(void *ptr)
{
	(void)ptr;
	test.foo = 3;
	test.bar = 4;
	my_printf("On thread %d test = { foo = %d, bar = %d}\n", GetCurrentThreadId(), test.foo, test.bar);
	return 0;
}

EXTERN_C int __stdcall mainCRTStartup(void)
{
	test.foo = 1;
	test.bar = 2;
	my_printf("On thread %d test = { foo = %d, bar = %d}\n", GetCurrentThreadId(), test.foo, test.bar);
	HANDLE thread = CreateThread(NULL, 0, thread_entry_point, NULL, 0, NULL);
	WaitForSingleObject(thread, INFINITE);
	my_printf("On thread %d test = { foo = %d, bar = %d}\n", GetCurrentThreadId(), test.foo, test.bar);
	return 0;
}
