#include "test.h"

Test::Test(string fname) {
	ReadModel();
	Mat subject = LoadSubject(fname);
	Mat coord = CalcCoordinate(subject);
	int closesti = FindClosest(coord); 
	Reconstruct(coord);
}

void Test::ReadModel() {
	model=Utility::ReadLog("eigen_output/eigen_vector.txt");
}

Mat Test::LoadSubject(string fname) {
	Mat subject = Mat::zeros(Size(OUTPUT_W*OUTPUT_H,1),CV_8U);
	Mat newsrc = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
	if (newsrc.empty()) {
		printf(" Error opening image\n");
		return;
	}
	int dimIdx;
	for (int j = 0; j < OUTPUT_H; j++) {
		for (int k = 0; k < OUTPUT_W; k++) {
			dimIdx = j*OUTPUT_W + k;
			subject.at<uchar>(0, dimIdx) = newsrc.at<uchar>(j, k);
		}
	}
	return subject;
}

Mat Test::CalcCoordinate(Mat subject) {
	Mat coord = model.t()*subject;
	return coord;
}

int Test::FindClosest(Mat coord) {
	double dist, maxdist;
	int maxi;
	for (int i = 1; i <= MAX_IMAGE_NUMBER; i++) {
		dist = CalcDistance(coord, i);
		if (i == 1) {
			maxdist = dist;
			maxi = 1;
		}
		else {
			if (dist > maxdist) {
				maxdist = dist;
				maxi = i;
			}
		}
	}
	return maxi;
}

double Test::CalcDistance(Mat coord, int sindex) {
	Mat tempsub = LoadSubject("facedb/s"+to_string(sindex)+".jpg");
	Mat tempcoord = CalcCoordinate(tempsub);
	
	int w = coord.size().width;
	int h = coord.size().height;
	double sqrsum;
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			sqrsum += (tempcoord.at<double>(i, j) - coord.at<double>(i, j))*(tempcoord.at<double>(i, j) - coord.at<double>(i, j));
		}
	}

	return sqrt(sqrsum);
}

void Test::Reconstruct(Mat coord) {
	Mat rec = model*coord;
	imshow("reconstruction",rec);
}