#include "train.h"

Train::Train() {
	if (!Utility::FileExist("eigen_output/eigen_vector.txt")) {
		InitMatrix(src);
		calcCovarMatrix(src, covar, means, CV_COVAR_NORMAL | CV_COVAR_ROWS);
		GenEigenVV(dst);
	}
	else {
		cout << "eigen vectors are already calculated" << endl;
	}
	
}

void Train::GenEigenVV(Mat & dst){
	Mat eigenvec_full;
	eigen(covar, eigenval, eigenvec_full);
	int dimtotal = eigenvec_full.size().height;
	eigenDimSz = CalcEigenDimSz(eigenval);
	for (int i = 0; i < eigenDimSz; i++) {
		cout << eigenval.at<double>(i, 0) << endl;
	}

	eigenvec = eigenvec_full(Rect(0,0,OUTPUT_W*OUTPUT_H, eigenDimSz));
	Utility::Log(eigenvec, "eigen_output/eigen_vector.txt");
	Utility::Normalize(eigenvec, eigenDimSz);
	Reformat(eigenvec, dst, eigenDimSz);
 	imshow("eigenfaces",dst);
	//Utility::Log(dst, "eigen_output/eigen_face.txt");
	Utility::Log(eigenval, "eigen_output/eigen_value.txt");
}

int Train::CalcEigenDimSz(Mat eigenval) {
	double energysum = 0;
	double energytotalsum = 0;
	int count;
	for (int i=0; i < eigenval.size().height; i++) {
		energytotalsum += eigenval.at<double>(i, 0);
	}
	for (count = 1; count - 1 < eigenval.size().height; count++) {
		energysum += eigenval.at<double>(count - 1, 0);
		if (energysum/energytotalsum >= ENERGY_R) {
			break;
		}
	}
	return count;
}

void Train::Reformat(Mat mat, Mat & dst, int eigenDimSz) {
	Utility::PixelVectorToMatrix(mat, dst, eigenDimSz);
}


void Train::InitMatrix(Mat & dst) {
	Size sz = Size(OUTPUT_H*OUTPUT_W, MAX_IMAGE_NUMBER*MAX_TEST_INDEX);
	dst = Mat::zeros(sz,CV_64F);
	Mat newsrc;
	Mat meanFace = Mat::zeros(Size(OUTPUT_W*OUTPUT_H,1),CV_64F);
	for (int i = 1; i <= MAX_IMAGE_NUMBER; i++) {
		for (int q = 1; q <= MAX_TEST_INDEX; q++) {
			char* fname = Utility::StringToChar("facedb/s" + to_string(i) + "_"+ to_string(q)+ ".jpg");
			newsrc = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
			if (newsrc.empty()) {
				printf(" Error opening image\n");
				return;
			}

			int dimIdx;
			int imgIdx;
			for (int j = 0; j < OUTPUT_H; j++) {
				for (int k = 0; k < OUTPUT_W; k++) {
					dimIdx = j*OUTPUT_W + k;
					imgIdx = (i-1)*MAX_TEST_INDEX + q-1;
					dst.at<double>(imgIdx, dimIdx) = (double)newsrc.at<uchar>(j, k);
					meanFace.at<double>(0, dimIdx) += (double)newsrc.at<uchar>(j, k);
				}
			}
		}
		
	}
	meanFace /= (MAX_IMAGE_NUMBER*MAX_TEST_INDEX);
	Utility::Log(meanFace, "eigen_output/meanface.txt");
	for (int i = 1; i <= MAX_IMAGE_NUMBER*MAX_TEST_INDEX; i++) {
		for (int j = 0; j < OUTPUT_H; j++) {
			for (int k = 0; k < OUTPUT_W; k++) {
				int dimIdx = j*OUTPUT_W + k;
				dst.at<double>(i - 1, dimIdx) -= meanFace.at<double>(0, dimIdx);
			}
		}
	}
}

