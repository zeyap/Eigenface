#include "mainwindow.h"
static Mat src;
static Mat display;
static Rect roiRect;
static Point seed;
static Mat anchors;
float scale;
bool alignCenter;

char window_name[] = "Eigenface";

MainWindow::MainWindow() {

	namedWindow(window_name, WINDOW_NORMAL|WINDOW_KEEPRATIO);
	resizeWindow(window_name, Size(WINDOW_WIDTH, WINDOW_HEIGHT));
	ReadImage();
	scale = 1.0f;
	alignCenter = true;
	CropAndScale(display,&scale,Rect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));
	setMouseCallback(window_name, OnMouse, 0);

	waitKey(0);
}

void MainWindow::ReadImage() {
	const char filename[] = "faces/1.pgm";
	src = imread(filename, IMREAD_COLOR);
	if (src.empty()) {
		printf(" Error opening image\n");
		printf(" Usage: ./Smoothing [image_name -- default ..lena.bmp] \n");
		return;
	}
	copyMakeBorder(src,display,(WINDOW_HEIGHT*2 -src.size().height)/2, (WINDOW_HEIGHT*2 - src.size().height) / 2, (WINDOW_WIDTH*2 - src.size().width) / 2, (WINDOW_WIDTH*2 - src.size().width) / 2, BORDER_CONSTANT,Scalar(0,0,0));
}

Mat MainWindow::CropAndScale(Mat mat, float* scale, Rect newRect) {
	Mat newMat = mat.clone();
	roiRect = newRect;
	if (*scale < 0.8f) {
		*scale = 0.8f;
	}
	else if (*scale > 2.0f) {
		*scale = 2.0f;
	}
	roiRect.width= newRect.width/(*scale);
	roiRect.height = newRect.height/(*scale);

	if (roiRect.x < 0) {
		roiRect.x = 0;
	}
	if (roiRect.y < 0) {
		roiRect.y = 0;
	}
	if (roiRect.x + roiRect.width > mat.size().width)
	{
		roiRect.x = mat.size().width - roiRect.width;
	}
	if (roiRect.y + roiRect.height > mat.size().height)
	{
		roiRect.y = mat.size().height - roiRect.height;
	}

	if (alignCenter) {
		roiRect.x = (mat.size().width - roiRect.width) / 2;
		roiRect.y = (mat.size().height - roiRect.height) / 2;
		alignCenter = false;
	}
	

	Mat roi = newMat(roiRect);
	DisplayAnchors(roi);
	imshow(window_name, roi);
	return roi;
}

void MainWindow::OnMouse(int event, int x, int y, int flags, void* ustc) {
	Rect newRect= roiRect;
	seed = Point(newRect.width/2, newRect.height / 2);
	int dx = x - seed.x;
	int dy = y - seed.y;
	if (event == EVENT_LBUTTONDOWN) {
		if (abs(dx)>WINDOW_WIDTH / 5) {
			newRect = Rect(newRect.x + dx*0.03f, newRect.y, WINDOW_WIDTH, WINDOW_HEIGHT);
		}
		if (abs(dy)>WINDOW_HEIGHT / 5) {
			newRect = Rect(newRect.x, newRect.y + dy*0.03f, WINDOW_WIDTH, WINDOW_HEIGHT);
		}
	}
	
	

	if (event == EVENT_MOUSEWHEEL) {
		scale+=getMouseWheelDelta(flags)/2000.0f;
	
	}

	if (event == EVENT_RBUTTONDOWN) {
		alignCenter = true;
	}

	newRect = Rect(newRect.x, newRect.y, WINDOW_WIDTH, WINDOW_HEIGHT);

	CropAndScale(display, &scale, newRect);

}

void MainWindow::InitAnchors(Mat & mat) {
	Scalar color = Scalar(255, 255, 255);
	anchors = Mat::zeros(mat.size(), mat.type());
	putText(anchors, "+", Point(mat.size().width *0.40, mat.size().height *0.50), FONT_HERSHEY_PLAIN, 2/scale, color, 1, 8);
	putText(anchors, "+", Point(mat.size().width *0.60, mat.size().height *0.50), FONT_HERSHEY_PLAIN, 2 / scale, color, 1, 8);
	
	putText(anchors, "Press [Enter] to confirm", Point(mat.size().width *0.05, mat.size().height *0.05), FONT_HERSHEY_PLAIN, 1 / scale, color, 1, 8);
	putText(anchors, "Right click to center", Point(mat.size().width *0.05, mat.size().height *0.10), FONT_HERSHEY_PLAIN, 1 / scale, color, 1, 8);
	putText(anchors, "<", Point(mat.size().width *0.05, mat.size().height *0.50), FONT_HERSHEY_PLAIN, 3 / scale, color, 1, 8);
	putText(anchors, ">", Point(mat.size().width *0.95, mat.size().height *0.50), FONT_HERSHEY_PLAIN, 3 / scale, color, 1, 8);
	putText(anchors, "^", Point(mat.size().width *0.50, mat.size().height *0.05), FONT_HERSHEY_PLAIN, 3 / scale, color, 1, 8);
	putText(anchors, "v", Point(mat.size().width *0.50, mat.size().height *0.95), FONT_HERSHEY_PLAIN, 3 / scale, color, 1, 8);

}
void MainWindow::DisplayAnchors(Mat & mat) {
	InitAnchors(mat);
	addWeighted(mat, 1.0, anchors, 1.0, 0.0, mat);
	
}