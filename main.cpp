#include "main.h"
#include <iostream>
#include <filesystem>
#include <elfio/elfio.hpp>
#include <vector>
using namespace ELFIO;
using namespace std;

int main(int argc, char* argv[]) {
	if (argc < 2) {
		cout << "No filename supplied\n";
		return 1;
	}
	string filePath = filesystem::current_path().u8string() + "/";
	string fileName = argv[1];
	fileName.resize(fileName.length() - 2);

	string command = "gcc -c -x c -O3 " + filePath + fileName + ".c -o " + filePath + fileName + ".o";
	system(command.c_str());
	//getfuncs(filePath + fileName + ".o");
	return 0;
}
void writeSymbolSuffix(string filename, string suffix) {
	vector<string> funcs;
	elfio reader;
	cout << "Reading " << filename << '\n';
	if (!reader.load(filename ) ) {
		cout << "Can't find or process ELF file " << filename << '\n';
		exit(2);
	}
	//Identify .text section of ELF
	Elf_Half sectionsCount = reader.sections.size();
	cout << "Number of sections: " << sectionsCount << '\n';
	int secIndex = 0;
	for (secIndex = 0; secIndex < sectionsCount; secIndex++ ) {
		const section* sec = reader.sections[secIndex];
		if (sec->get_type() == SHT_SYMTAB)
			break;
	}
	section* sec = reader.sections[secIndex];
	if (secIndex >= sectionsCount) {		
		cout << "Unable to find symtable in " << filename << '\n';
		exit(2);
	}
	//Access table
	symbol_section_accessor* table = new symbol_section_accessor(reader,sec);

	//access elements
	string name;
	Elf64_Addr value;
	Elf_Xword size;
	unsigned char bind;
	unsigned char type;
	Elf_Half section_index;
	unsigned char other;
	for (Elf_Xword sI = 0; sI < table->get_symbols_num(); sI++) {
		table->get_symbol(sI, name, value, size, bind, type, section_index, other);
		cout << '[' << sI << "] " << name << ' ' << (int)value << ' ' << (int)size << ' ' << (int)bind << ' ' << (int)type << ' ' <<(int) section_index << ' ' << (int)other << '\n';
		if ((int)type == 2) {
			//Identify function definitions
			name = name + prefix;
			





			
			//There's no way to actually write a change to the table.










		}
	}
	return funcs;
	/*
	get_symbol(
		Elf_Xword index,
		std::string& name,
		Elf64_Addr& value,
		Elf_Xword& size,
		unsigned char& bind,
		unsigned char& type,
		Elf_Half& section_index,
		unsigned char& other)
	*/
}
