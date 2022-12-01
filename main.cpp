#include "main.h"
#include <iostream>
#include <filesystem>
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
		std::cout << "No filename supplied\n";
		return 1;
	}
    std::string frag1 ("gcc -c -x c -O3 ");
    std::string frag2 (".c -o ");
    std::string frag3 (".o");

	std::string cwd = std::filesystem::current_path().u8string() + "/";
	std::string funcName = argv[1];
	funcName.resize(funcName.length() - 2);

	std::string command = frag1 + cwd + funcName + frag2 + cwd + funcName + frag3;
	system(command.c_str());
	return 0;
}