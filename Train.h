#ifndef TRAIN_H
#define TRAIN_H

//<dimtotal

#include "main.h"
#include "utility.hpp"

class Train {
public:
	Train();
private:
	void InitMatrix(Mat & dst);
	void GenEigenVV();
	void Norm(Mat & mat);
	void Reformat(Mat mat, Mat & dst);
	Mat src;
	Mat covar;
	Mat means;
	Mat eigenval;
	Mat eigenvec;
	Mat dst;
};

#endif