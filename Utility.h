#ifndef UTILITY_H
#define UTILITY_H
#include <iostream>
#include <fstream>
using namespace std;

class Utility {
public:
	static char* StringToChar(string str) {
		char* c = new char[str.length() + 1];
		strcpy(c, str.c_str());
		return c;
	}
	static bool FileExist(string str) {
		string newStr = str;
		std::ifstream fin(newStr);
		if (fin) {
			return true;
		}
		else {
			return false;
		}
	}
	static void Log() {

	}
};

#endif