#pragma once

#include <stdint.h>

typedef struct Test {
	int64_t foo;
	int64_t bar;
} Test;

typedef void printf_func(const char *fmt, ...);
typedef void tls_dll_entry_func(printf_func *printf);
