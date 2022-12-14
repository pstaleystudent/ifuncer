# Purpose:
During compilation, automatically convert a C source file to support multiple levels of SIMD processor support using GCC's ifunc.
# Usage:
Takes the place of the compilation stage of GCC, producing a .o file.
ifuncer must be stored in the same directory as _resolver.c

Example with a makefile, where 'function.c' should have multiple implementations:
```
main:	main.c function.o
	gcc -Wall main.c function.o -o main
function.o:	function.c
	[path_to_ifuncer]/ifuncer function.c
```
# Details:
- Produces implementations for SVE2, SVE, Advanced SIMD, and standard ARM64.
- Only supports building for ARM64.
- Supports absolute and relative file paths.
- Supports output file name specification (i.e. 'ifuncer function.c -o newname.o').
- Can probably produce side effects with other non-standard compilation tools.
- Will probably fail with function names containing '$' or with a function called "T".
