#ifndef TRAIN_H
#define TRAIN_H

#define ENERGY_R 0.95

//<dimtotal

#include "main.h"
#include "utility.hpp"

class Train {
public:
	Train();
	static void Reformat(Mat mat, Mat & dst, int eigenDimSz);
private:
	void InitMatrix(Mat & dst);
	void GenEigenVV(Mat & dst);
	int CalcEigenDimSz(Mat eigenval);
	Mat src;
	Mat covar;
	Mat means;
	Mat eigenval;
	Mat eigenvec;
	Mat dst;
	int eigenDimSz;
};

#endif