#include "main.h"
#include <iostream>
#include <filesystem>
#include <vector>
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <cstring>
#include <array>
#include <fstream>
#include <unistd.h>
using namespace std;

int main(int argc, char* argv[]) {
	if (argc < 2)
		cout << "No filename supplied\n";
	if (argc < 2 || argc > 4 || (argc > 2  &&strcmp(argv[2],"-o") != 0)) {
		cout << "Usage: ifuncer filename [-o outputloc]\n";
		return 1;
	}
	bool out_path_specified = (argc == 4);
	string delims = " \n";
	string filePath = filesystem::current_path().u8string() + "/";
	string fileName = argv[1];
	string outPath;
	string outName;
	if (fileName[0] == '/') {
		//absolute in path
		std::__cxx11::basic_string<char>::size_type lastSlashLoc = 0;
		for (std::__cxx11::basic_string<char>::size_type i = 1; i < fileName.length(); i++)
			if (fileName[i] == '/')
				lastSlashLoc = i;
		filePath = fileName;
		filePath.resize(lastSlashLoc + 1);
		fileName.erase(lastSlashLoc);
	}
	outPath = filePath;
	if (out_path_specified) {
		outName = argv[3];
		if (outName[0] == '/') {
			//absolute out path
			std::__cxx11::basic_string<char>::size_type lastSlashLoc = 0;
			for (std::__cxx11::basic_string<char>::size_type i = 1; i < outName.length(); i++)
				if (outName[i] == '/')
					lastSlashLoc = i;
			outPath = outName;
			outPath.resize(lastSlashLoc + 1);
			outName.erase(lastSlashLoc);
		}
	} else
		outName = fileName;
    outName.resize(outName.length() - 2);
	fileName.resize(fileName.length() - 2);
	//capture symbol table
	string command = "gcc -c -x c -O3 " + filePath + fileName + ".c -o " + outPath + outName + ".o";
	system(command.c_str());
	command = "nm " + outPath + outName + ".o";
	string symbolTable = exec(command.c_str());
	vector<string*> table = tokenize(symbolTable, delims);

	//capture names
	vector<string*> functions;
	for (long unsigned int i = 1; i < table.size() - 1; i+=1) {
		if (table[i]->compare("T") == 0) {
			functions.push_back(table[i + 1]);
		}
	}
	//------------------------------------------------COMPILE------------------------------------------------
	//SVE2
	string argument;
	for (long unsigned int i = 0; i < functions.size(); i++) {
		argument = argument + " -D" + *functions[i] + "=_IFUNCER_" + *functions[i] + "_SVE2";
	}
	command = "gcc -c -x c -O3 -march=armv8-a+sve2" + argument + " " + filePath + fileName 
		+ ".c -o " + outPath + "_IFUNCER_" + outName + "_SVE2"+ ".o";
	cout << command << '\n';
	system(command.c_str());
	//SVE
	argument = "";
	for (long unsigned int i = 0; i < functions.size(); i++) {
		argument = argument + " -D" + *functions[i] + "=_IFUNCER_" + *functions[i] + "_SVE";
	}
	command = "gcc -c -x c -O3 -march=armv8-a+sve" + argument + " " + filePath + fileName 
		+ ".c -o " + outPath + "_IFUNCER_" + outName + "_SVE"+ ".o";
	cout << command << '\n';
	system(command.c_str());
	//Advanced SIMD
	argument = "";
	for (long unsigned int i = 0; i < functions.size(); i++) {
		argument = argument + " -D" + *functions[i] + "=_IFUNCER_" + *functions[i] + "_SIMD";
	}
	command = "gcc -c -x c -O3 -march=armv8-a+simd" + argument + " " + filePath + fileName 
		+ ".c -o " + outPath + "_IFUNCER_" + outName + "_SIMD"+ ".o";
	cout << command << '\n';	
	system(command.c_str());
	//Standard/NO SIMD
	argument = "";
	for (long unsigned int i = 0; i < functions.size(); i++) {
		argument = argument + " -D" + *functions[i] + "=_IFUNCER_" + *functions[i] + "_NONE";
	}
	command = "gcc -c -x c -O3 " + argument + " " + filePath + fileName 
		+ ".c -o " + outPath + "_IFUNCER_" + outName + "_NONE"+ ".o";
	cout << command << '\n';
	system(command.c_str());
	
	//------------------------------------------------IFUNC------------------------------------------------
	char pathBuffer[256];
	readlink("/proc/self/exe", pathBuffer, 255);
	string sourcePath = pathBuffer;
	for (int i = sourcePath.size() - 1; i >= 0; i--) {
		if (sourcePath[i] == '/') {
			sourcePath.resize(i + 1);
			break;
		}
	}
	for (long unsigned int i = 0; i < functions.size(); i++) {
		string fName = *functions[i];
		replaceFile(sourcePath + "_resolver.c", outPath + "_IFUNCER_RESOLVE_" + fName + ".c", "$", fName);
		command = "gcc -c -x c " + outPath + "_IFUNCER_RESOLVE_" + fName + ".c";
		cout << command << '\n';
		system(command.c_str());
	}
	//whew, laddie
	command = "ld -r -o " + outName + ".o";
	for (long unsigned int i = 0; i < functions.size(); i++)
		command = command + " " + outPath + "_IFUNCER_RESOLVE_" + *functions[i] + ".o";
	command = command + " " + outPath + "_IFUNCER_" + outName + "_NONE"+ ".o";
	command = command + " " + outPath + "_IFUNCER_" + outName + "_SIMD"+ ".o";
	command = command + " " + outPath + "_IFUNCER_" + outName + "_SVE"+ ".o";
	command = command + " " + outPath + "_IFUNCER_" + outName + "_SVE2"+ ".o";
	cout << command << '\n';
	system(command.c_str());
	cleanup(outPath);
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
void replaceFile(string f1, string f2, string oldString, string newString) {
    ifstream in(f1);
    ofstream out(f2);
	if (!in)
        cout << "Could not open in\n";
	if (!out)
        cout << "Could not open out\n";
    string line;
    size_t len = oldString.length();
    while (getline(in, line)) {
        while (true) {
            size_t pos = line.find(oldString);
            if (pos != string::npos)
                line.replace(pos, len, newString);
            else 
                break;
        }
		out << line << '\n';
    }
}
void cleanup(string path) {
	string c = "rm " + path + "_IFUNCER_*";
	system(c.c_str());
}