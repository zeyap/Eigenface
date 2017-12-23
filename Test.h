#ifndef TEST_H
#define TEST_H

#include "main.h"
#include "utility.hpp"
#include "train.h"

class Test {
public:
	Test(string subject_fname, int PCs);
	Test::Test() {};
	int closestCandidate;
protected:
	void ReadModel(int PCs);
	void ReadMeanFace();
	Mat LoadSubject(string fname);
	Mat model;
	Mat meanFace;
	Mat CalcCoordinate(Mat subject);
	int FindClosest(Mat coord);
	double CalcDistance(Mat coord, int sindex, int imgindex);
};

#endif
