make:
	nasm -f win64 hook.asm -o hook.o
	x86_64-w64-mingw32-gcc -masm=intel -Wall etwti-hook.c hook.o -o etwti-hook.exe -m64 -lmsvcrt -lntdll -ldbghelp

clean:
	rm -rf *.o
