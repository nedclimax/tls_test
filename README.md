# tls_test
Testing thread local storage (TLS) on windows when not linking with the C runtime

This can be done because the initialization of POD (Plain-Old-Data) structs is handled by the OS loader on windows (see ["The .tls Section" in the PE documentation](https://learn.microsoft.com/en-us/windows/win32/debug/pe-format#the-tls-section))
