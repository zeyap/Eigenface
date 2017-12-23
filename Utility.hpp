#ifndef UTILITY_H
#define UTILITY_H
#include "main.h"
#include <fstream>

class Utility {
public:
	Utility() {};
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
			cout << "File " + fname + " already exists" << endl;
			waitKey();
			return;
		}
		ofstream outfile(fname);
		int w, h;
		w = mat.size().width;
		h = mat.size().height;
		for (int i = 0; i < h; i++) {
			for (int j = 0; j < w; j++) {
				string tempc = to_string(mat.at<double>(i, j));
				outfile<< tempc;
				if (j < w - 1) {
					outfile<<',';
				}
				else {
					outfile<<endl;
				}
			}
		}
		outfile.close();
	}
	static Mat ReadLog(string fname) {
		Mat log;
		vector <vector<double>> data;
		ifstream infile(fname);
		while (infile) {
			string sinline;
			if (!getline(infile, sinline)) break;

			vector <double> recordinline;
			istringstream sstream(sinline);//split on commas
			while (sstream) {
				string sinline;
				if (getline(sstream, sinline, ',')) {
					recordinline.push_back(string_to_double(sinline));
				}
				else {
					break;
				}
			}
			data.push_back(recordinline);
		}
		if (!infile.eof()) {
			cerr << "fooey!" << endl;
		}

		int w=data[0].size();
		int h=data.size();
		log= Mat::zeros(Size(w,h), CV_64F);
		for (int i = 0; i < h; i++) {
			for (int j = 0; j < w; j++) {
				log.at<double>(i, j) = data[i][j];
			}
		}
		data.clear();
		return log;
	}
	static void PixelVectorToMatrix(Mat src, Mat & dst, int eigendimSz) {
		dst = Mat::zeros(Size(eigendimSz*OUTPUT_W, OUTPUT_H), CV_8U);
		Mat dst0 = Mat::zeros(Size(eigendimSz*OUTPUT_W, OUTPUT_H), src.type());

		for (int i = 0; i < eigendimSz; i++) {
			Rect srcrect, dstrect;
			for (int j = 0; j < OUTPUT_H; j++) {
				srcrect = Rect(j*OUTPUT_W, i, OUTPUT_W, 1);
				dstrect = Rect(i*OUTPUT_W, j, OUTPUT_W, 1);
				Mat srcroi = src(srcrect);
				Mat dstroi = dst0(dstrect);
				srcroi.convertTo(dstroi, dstroi.type(), 1, 0);
			}
			for (int j = 0; j < OUTPUT_H; j++) {
				for (int k = 0; k < OUTPUT_W; k++) {
					dst.at<uchar>(j, i*OUTPUT_W+k) = (uchar)(dst0.at<double>(j, i*OUTPUT_W + k)*255);
				}
			}
		}
	}

	static void Normalize(Mat & mat, int eigendimSz) {
		//normalize dimension values (in double) within each sample
		int dimSz = OUTPUT_H*OUTPUT_W;
		for (int i = 0; i < eigendimSz; i++) {
			double max = -9999.0;
			double min = 9999.0;
			for (int j = 0; j < dimSz; j++) {
				double temp = mat.at<double>(i, j);
				if (temp > max) {
					max = temp;
				}
				if (temp < min) {
					min = temp;
				}
			}

			for (int j = 0; j < dimSz; j++) {
				mat.at<double>(i, j) = (mat.at<double>(i, j) - min) / (max - min);
			}
		}
	}

private:
	static double Utility::string_to_double(const std::string& s)
	{
		stringstream sstream(s);
		double x=0;
		sstream >> x;
		return x;
	}

};

#endif