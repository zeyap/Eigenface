#ifndef TRAIN_H
#define TRAIN_H

//<dimtotal

#include "main.h"
#include "utility.hpp"

class Train {
public:
	Train();
	static void Reformat(Mat mat, Mat & dst);
private:
	void InitMatrix(Mat & dst);
	void GenEigenVV();
	Mat src;
	Mat covar;
	Mat means;
	Mat eigenval;
	Mat eigenvec;
	Mat dst;
};

#endif