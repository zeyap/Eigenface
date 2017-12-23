#include "reconstruct.h"

Reconstruct::Reconstruct(string subject_fname, int PCs) {
	ReadModel(PCs);
	ReadMeanFace();
	Mat subject = LoadSubject(subject_fname);
	Mat coord = CalcCoordinate(subject);
	
	Mat rec = model.t()*coord.t();
	Mat rec_t = rec.t();
	rec_t += meanFace;
	Mat newrec;
	Utility::Normalize(rec_t, 1);
	Utility::PixelVectorToMatrix(rec_t, newrec, 1);
	imshow("reconstruction", newrec);
}