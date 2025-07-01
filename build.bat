@echo off
setlocal
pushd %~dp0
set cflags=/nologo /FC /MT /utf-8 /W4 /WX /Fo.\obj\ /GR- /EHa- /GS-
set cflags=%cflags% /Z7 /Oi /Gy /fp:except- /fp:fast
set lflags=/incremental:no /opt:ref,icf /subsystem:console /merge:.CRT=.rdata
set lflags=%lflags% libvcruntime.lib kernel32.lib user32.lib gdi32.lib
if not exist build mkdir build
if not exist build\obj mkdir build\obj
pushd build
del *.pdb >NUL 2>&1
cl %cflags% ..\tls_test.c /link %lflags%
cl %cflags% ..\tls_dll_test.c /LD /link %lflags%
cl %cflags% ..\tls_test2.cpp /link %lflags% libucrt.lib
cl %cflags% /TP /Fetls_test3 ..\tls_test.c /link %lflags%
popd
popd
