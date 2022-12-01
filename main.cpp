#include "main.h"
#include <iostream>
/*		todo:
	capture arguments
	compile base and SIMD objects
	rename base and SIMD object functions
	generate ifunc resolver
	compile resolver object
		
	main.o: main.c
		gcc -c -g -O3 main.c
	func: func.c main.o
		ifuncer func.c
	ld func.o main.o -o exampleprogram
		
	*/
int main(int argc, char* argv[]) {
	if (argc < 2) {
		std::cout << "No filename supplied";
		return 1;
	}
	std::string testfunc = "gcc - c - O3 ";
	testfunc.append(argv[1]).append(" o func.o");
	system(testfunc.c_str());
	return 0;
}