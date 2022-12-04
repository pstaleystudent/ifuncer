#include <string>
#include <vector>
using namespace std;
#pragma once
/*
todo:
capture arguments
compile base and SIMD objects
rename base and SIMD object functions
generate ifunc resolver
compile resolver object
merge base, SIMD, resolver objects with ld
cleanup
provide template makefile
documentation

extensions:
output location switch
failed vectorization notification
arbitrary arguement switches
remove failed vectorization
toggle archetecture switch
arbitrary optimization switches
*/
int main(int argc, char* argv[]);
//vector<string> getfuncs(string filename);
std::string exec(const char* cmd);
vector<string*> tokenize (string& in, string& delims);
void replaceFile(string f1, string f2, string oldString, string newString);
void cleanup(string path);