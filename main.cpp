#include "main.h"
#include <iostream>
#include <filesystem>
#include <vector>
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
using namespace std;

int main(int argc, char* argv[]) {
	if (argc < 2) {
		cout << "No filename supplied\n";
		return 1;
	}
	string delims = " \n";
	string filePath = filesystem::current_path().u8string() + "/";
	string fileName = argv[1];
	fileName.resize(fileName.length() - 2);

	//capture symbol table
	string command = "gcc -c -x c -O3 " + filePath + fileName + ".c -o " + filePath + fileName + ".o";
	system(command.c_str());
	command = "nm " + filePath + fileName + ".o";
	string symbolTable = exec(command.c_str());
	vector<string*> table = tokenize(symbolTable, delims);

	//capture names
	vector<string*> functions;
	for (long unsigned int i = 1; i < table.size() - 1; i+=3)
		if ((table[i])[0] == "T")
			functions.push_back(table[i + 1]);
	

	//SVE2
	string argument;
	for (long unsigned int i = 0; i < functions.size(); i++) {
		argument = argument + " -D" + *functions[i] + "=_IFUNCER_" + *functions[i] + "_SVE2";
	}
	command = "gcc -c -x c -O3 -march=armv8-a+sve2" + argument + " " + filePath + fileName 
		+ ".c -o " + filePath + "_IFUNCER_" + fileName + "_SVE2"+ ".o";
	system(command.c_str());

	//SVE/NEON
	argument = "";
	for (long unsigned int i = 0; i < functions.size(); i++) {
		argument = argument + " -D" + *functions[i] + "=_IFUNCER_" + *functions[i] + "_SVE";
	}
	command = "gcc -c -x c -O3 -march=armv8-a+sve" + argument + " " + filePath + fileName 
		+ ".c -o " + filePath + "_IFUNCER_" + fileName + "_SVE"+ ".o";
	system(command.c_str());

	//Advanced/NEON
	argument = "";
	for (long unsigned int i = 0; i < functions.size(); i++) {
		argument = argument + " -D" + *functions[i] + "=_IFUNCER_" + *functions[i] + "_NEON";
	}
	command = "gcc -c -x c -O3 -march=armv8-a+simd" + argument + " " + filePath + fileName 
		+ ".c -o " + filePath + "_IFUNCER_" + fileName + "_NEON"+ ".o";
	system(command.c_str());

	return 0;
}

string exec(const char* cmd) {
    array<char, 128> buffer;
    string result;
    unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw runtime_error("Pipe failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}
vector<string*> tokenize (string& in, string& delims) {
	vector<string*> out;
	if (in.size() < 1)
		return out;
	string* s = new string();
	bool ondelims = true;
	for (long unsigned int i = 0; i < in.size(); i++) {
		if (delims.find(in[i]) == std::string::npos) {
			s->push_back(in[i]);
			ondelims = false;
		} else if (!ondelims) {
			ondelims = true;
			out.push_back(s);
			s = new string();
		}
	}
	if (!ondelims)
		out.push_back(s);
	if (out.size() == 0)
		delete s;
	return out;
}