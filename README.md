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
	ifuncer function.c
```
# Limitations:
- This is a prototype more than anything.
- Produces implementations for SVE2, SVE, ASIMD, and standard.
- Only supports building for Linux ARM64.
- Currently does not support any options, nor absolute file paths as input.
- May require that function names in the file are not used elsewhere in the file? Not tested.
- Can probably produce side effects with other GCC tools, only intended for use with standard functions.
- If you somehow used a $ in a function name, this will probably fill up your hard drive.
