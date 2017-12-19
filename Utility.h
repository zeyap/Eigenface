#ifndef UTILITY_H
#define UTILITY_H

#include "main.h"
#include <fstream>

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

	static void Log(Mat mat, string fname) {
		if (FileExist(fname)) {
			cout <<"File "+ fname +" already exists" << endl;
			waitKey();
			return;
		}
		ofstream outfile(fname);
		int w, h;
		w = mat.size().width;
		h = mat.size().height;
		string buffer;
		for (int i = 0; i < h; i++) {
			for (int j = 0; j < w; j++) {
				string tempc = to_string(mat.at<double>(i, j));
				buffer += tempc ;
				if (j < 2 * w - 1) {
					buffer += ',';
				}
				else {
					buffer += ';/n';
				}
			}
		}
		outfile << buffer;
		outfile.close();
	}

};

#endif