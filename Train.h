#ifndef TRAIN_H
#define TRAIN_H

#define EIGEN_DIM_SZ 10 //<dimtotal

#include "main.h"
#include "utility.h"

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