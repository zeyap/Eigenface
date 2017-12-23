#include "test.h"

Test::Test(string subject_fname, int PCs) {
	ReadModel(PCs);
	ReadMeanFace();
	Mat subject = LoadSubject(subject_fname);
	Mat coord = CalcCoordinate(subject);
	closestCandidate = FindClosest(coord);
}

void Test::ReadMeanFace() {
	meanFace = Utility::ReadLog("eigen_output/meanFace.txt");
	Mat meanFaceNorm = meanFace.clone();
	Utility::Normalize(meanFaceNorm, 1);
	Mat meanFaceMat;
	Utility::PixelVectorToMatrix(meanFaceNorm, meanFaceMat, 1);
	imshow("meanFace", meanFaceMat);
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
	return subject-meanFace;
}

Mat Test::CalcCoordinate(Mat subject) {
	Mat coord = model*subject.t();
	return coord.t();
}

int Test::FindClosest(Mat coord) {
	double dist, mindist;
	int mini;
	for (int i = 1; i <= MAX_IMAGE_NUMBER; i++) {
		for (int j = 1; j < MAX_TEST_INDEX; j++) {
			dist = CalcDistance(coord, i,j);
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
		
	}
	return mini;
}

double Test::CalcDistance(Mat coord, int sindex, int imgindex) {
	Mat tempsub = LoadSubject("facedb/s"+to_string(sindex)+"_"+to_string(imgindex)+".jpg");
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