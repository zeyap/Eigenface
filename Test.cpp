#include "test.h"

Test::Test(string fname, int PCs) {
	ReadModel(PCs);
	Mat subject = LoadSubject(fname);
	Mat coord = CalcCoordinate(subject);
	closestCandidate = FindClosest(coord);
	Reconstruct(coord);
}

void Test::ReadModel(int PCs) {
	model=Utility::ReadLog("eigen_output/eigen_vector.txt");
	model = model(Rect(0,0,model.size().width,PCs));
}

Mat Test::LoadSubject(string fname) {
	Mat subject = Mat::zeros(Size(OUTPUT_W*OUTPUT_H,1),CV_64F);
	Mat newsrc = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
	if (newsrc.empty()) {
		printf(" Error opening image\n");
		return subject;
	}
	int dimIdx;
	for (int j = 0; j < OUTPUT_H; j++) {
		for (int k = 0; k < OUTPUT_W; k++) {
			dimIdx = j*OUTPUT_W + k;
			subject.at<double>(0, dimIdx) = (double)newsrc.at<uchar>(j, k);
		}
	}
	Utility::Normalize(subject,1);
	return subject;
}

Mat Test::CalcCoordinate(Mat subject) {
	Mat coord = model*subject.t();
	return coord.t();
}

int Test::FindClosest(Mat coord) {
	double dist, mindist;
	int mini;
	for (int i = 1; i <= MAX_IMAGE_NUMBER; i++) {
		dist = CalcDistance(coord, i);
		if (i == 1) {
			mindist = dist;
			mini = 1;
		}
		else {
			if (dist < mindist) {
				mindist = dist;
				mini = i;
			}
		}
	}
	return mini;
}

double Test::CalcDistance(Mat coord, int sindex) {
	Mat tempsub = LoadSubject("facedb/s"+to_string(sindex)+".jpg");
	Mat tempcoord = CalcCoordinate(tempsub);
	
	int w = coord.size().width;
	int h = coord.size().height;
	double sqrsum=0;
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			double temp = tempcoord.at<double>(i, j) - coord.at<double>(i, j);
			sqrsum += temp*temp;
		}
	}

	return sqrt(sqrsum);
}

void Test::Reconstruct(Mat coord) {
	Mat rec = model.t()*coord.t();
	Mat rec_t = rec.t();
	Utility::Normalize(rec_t, 1);
	Mat newrec;
	Utility::PixelVectorToMatrix(rec_t, newrec, 1);
	cout << newrec.at<uchar>(0,0);
	cout << newrec.at<uchar>(0, 1);
	cout << newrec.at<uchar>(0, 2);
	imshow("reconstruction",newrec);
}