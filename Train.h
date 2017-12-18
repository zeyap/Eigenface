#ifndef TRAIN_H
#define TRAIN_H

#include "main.h"

class Train {
public:
	Train();
private:
	void InitMatrix();
	Mat src;
	Mat covar;
	Mat mean;
};

#endif