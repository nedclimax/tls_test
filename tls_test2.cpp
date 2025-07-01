#define STB_SPRINTF_IMPLEMENTATION
#define STB_SPRINTF_NOFLOAT
#include "stb_sprintf.h"
#include <stdarg.h>
#include <Windows.h>

void my_printf(const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	int size = stbsp_vsnprintf(NULL, 0, fmt, args);
	char *buf = (char *)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, size + 1);
	stbsp_vsnprintf(buf, size + 1, fmt, args);
	va_end(args);
	HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);
	WriteConsole(hstdout, buf, size + 1, NULL, NULL);
	HeapFree(GetProcessHeap(), 0, buf);
}

struct Test {
	Test() {
		my_printf("Constructor called on thread %d.\n", GetCurrentThreadId());
	}
	~Test() {
		my_printf("Destructor called on thread %d.\n", GetCurrentThreadId());
	}
	int foo = 2;
};

// Here we require the CRT simply because this struct has a constructor and a destructor
__declspec(thread) Test test;

static DWORD thread_entry_point(void *ptr) {
	(void)ptr;
	test.foo = 5;
	my_printf("On the main thread test.foo is %d\n", test.foo);
	return 0;
}

EXTERN_C int __stdcall mainCRTStartup(void) {
	my_printf("On the main thread test.foo is %d\n", test.foo);
	HANDLE thread = CreateThread(NULL, 0, thread_entry_point, NULL, 0, NULL);
	WaitForSingleObject(thread, INFINITE);
	my_printf("On the main thread test.foo is still %d\n", test.foo);
	return 0;
}
