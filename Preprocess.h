#ifndef PREPROCESS_H
#define PREPROCESS_H

#define WINDOW_HEIGHT 350
#define WINDOW_WIDTH 350
#define IMAGE_SIDE_LEN 700

#define EYE_HEIGHT_R 0.5
#define EYE_WIDTH_R 0.2

#include "main.h"
#include "utility.hpp"

class Preprocess {
public:
	Preprocess(char* window_name);

private:
	void ReadImage(const char* filename);
	static void OnMouse(int event, int x, int y, int flags, void* ustc);
	static Mat CropAndScale(Mat mat, float* scale, Rect newRect);
	static void DisplayAnchors(Mat &mat);
	static void InitAnchors(Mat & mat);
	void SaveImage(Mat roi, int i);
};
#endif // !MAINWINDOW_H

