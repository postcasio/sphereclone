#include <sstream>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

namespace Files {
	const char* readTextFile(const char* filename) {
		std::ifstream file(filename);
		
		if (!file || !file.is_open()) {
			return NULL;
		}
		
		std::string string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		
		file.close();
		
		return string.c_str();
	}
}	