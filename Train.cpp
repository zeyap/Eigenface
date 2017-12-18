#include "train.h"

Train::Train() {
	InitMatrix();
}

void Train::InitMatrix() {
	Size sz = Size(MAX_IMAGE_NUMBER*OUTPUT_W, OUTPUT_H);
	Mat matrix = Mat::zeros(sz,CV_8U);
	Mat newsrc;
	for (int i = 1; i <= MAX_IMAGE_NUMBER; i++) {
		String str = "facedb/s" + to_string(i) + ".jpg";
		char* fname = new char[str.length() + 1];
		strcpy(fname, str.c_str());
		newsrc = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
		if (newsrc.empty()) {
			printf(" Error opening image\n");
			return;
		}

		Rect rect = Rect((i-1)*OUTPUT_W,0,OUTPUT_W,OUTPUT_H);
		Mat dstroi=matrix(rect);
		newsrc.convertTo(dstroi,dstroi.type(),1,0);
	}
	imshow("new", matrix);
}