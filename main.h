#include <string>
#include <vector>
using namespace std;
#pragma once
int main(int argc, char* argv[]);
//vector<string> getfuncs(string filename);
std::string exec(const char* cmd);
vector<string*> tokenize (string& in, string& delims);
void replaceFile(string f1, string f2, string oldString, string newString);
void cleanup(string path);