#include "train.h"

Train::Train() {
	InitMatrix(src);
	//imwrite("src.jpg", src);
	calcCovarMatrix(src,covar,means,CV_COVAR_NORMAL|CV_COVAR_ROWS);
	GenEigenVV();
	waitKey(100000);
}

void Train::GenEigenVV(){
	Mat eigenvec_full;
	eigen(covar, eigenval, eigenvec_full);
	int dimtotal = eigenvec_full.size().height;
	for (int i = 0; i < EIGEN_DIM_SZ; i++) {
		cout << eigenval.at<double>(i, 0) << endl;
	}

	eigenvec = eigenvec_full(Rect(0,0,OUTPUT_W*OUTPUT_H, EIGEN_DIM_SZ));
	Norm(eigenvec);
	Reformat(eigenvec);
	imshow("eigenvec",eigenvec);
}

void Train::Norm(Mat & mat) {
	//normalize dimension values (in double) within each sample
	int dimSz = OUTPUT_H*OUTPUT_W;
	for (int i = 0; i < EIGEN_DIM_SZ; i++) {
		double sum=0.0;
		for (int j = 0; j < dimSz; j++) {
			sum += mat.at<double>(i, j);
		}
		double m = sum / dimSz;
		
		double sqrSum = 0.0;
		for (int j = 0; j < dimSz; j++) {
			sqrSum += (mat.at<double>(i, j)-m)*(mat.at<double>(i, j) - m);
		}
		double stdDev = sqrt(sqrSum / dimSz);

		for (int j = 0; j < dimSz; j++) {
			mat.at<double>(i, j) = (mat.at<double>(i, j) - m) / stdDev;
		}
	}
}

void Train::Reformat(Mat & mat) {
	Mat mat_c = mat.clone();

	for (int i = 0; i < EIGEN_DIM_SZ; i++) {
		Rect srcrect, dstrect;
		for (int j = 0; j < OUTPUT_H; j++) {
			srcrect = Rect(j*OUTPUT_W, i, OUTPUT_W, 1);
			dstrect = Rect(i*OUTPUT_W, j, OUTPUT_W, 1);
			Mat srcroi = mat_c(srcrect);
			Mat dstroi = mat(dstrect);
			srcroi.convertTo(dstroi,dstroi.type(),1,0);
		}
	}
}


void Train::InitMatrix(Mat & dst) {
	Size sz = Size(OUTPUT_H*OUTPUT_W, MAX_IMAGE_NUMBER);
	dst = Mat::zeros(sz,CV_8U);
	Mat newsrc;
	for (int i = 1; i <= MAX_IMAGE_NUMBER; i++) {
		char* fname = Utility::StringToChar("facedb/s" + to_string(i) + ".jpg");
		newsrc = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
		if (newsrc.empty()) {
			printf(" Error opening image\n");
			return;
		}
		
		int dimIdx;
 		for (int j = 0; j < OUTPUT_H; j++) {
			for (int k = 0; k < OUTPUT_W; k++) {
				dimIdx = j*OUTPUT_W + k;
				dst.at<uchar>(i - 1, dimIdx) = newsrc.at<uchar>(j,k);
			}
			
		}
		
	}
}

