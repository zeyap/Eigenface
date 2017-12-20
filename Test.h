#ifndef TEST_H
#define TEST_H

#include "main.h"
#include "utility.hpp"

class Test {
public:
	Test(string fname);
private:
	void ReadModel();
	Mat LoadSubject(string fname);
	Mat model;
	Mat CalcCoordinate(Mat subject);
	int FindClosest(Mat coord);
	double CalcDistance(Mat coord, int sindex);
};

#endif
