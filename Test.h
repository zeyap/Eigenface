#ifndef TEST_H
#define TEST_H

#include "main.h"
#include "utility.hpp"
#include "train.h"

class Test {
public:
	Test(string fname,int PCs);
	int closestCandidate;
private:
	void ReadModel(int PCs);
	Mat LoadSubject(string fname);
	Mat model;
	Mat meanFace;
	Mat CalcCoordinate(Mat subject);
	int FindClosest(Mat coord);
	double CalcDistance(Mat coord, int sindex);
	void Reconstruct(Mat coord);
};

#endif
