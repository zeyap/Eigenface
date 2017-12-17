#include "mainwindow.h"
static Mat src;
static Mat dst;
static Rect roiRect;
static bool drag;
static Point seed;

char window_name[] = "Canny Edge Detector Demo";

MainWindow::MainWindow() {

	namedWindow(window_name, WINDOW_NORMAL|WINDOW_KEEPRATIO);
	resizeWindow(window_name, Size(WINDOW_WIDTH, WINDOW_HEIGHT));

	ReadImage();
	CropAndScale(src,1.0f,Rect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));
	setMouseCallback(window_name, OnMouse, 0);
	drag = false;

	waitKey(0);
}

void MainWindow::ReadImage() {
	const char filename[] = "gears.jpg";
	src = imread(filename, IMREAD_COLOR);
	if (src.empty()) {
		printf(" Error opening image\n");
		printf(" Usage: ./Smoothing [image_name -- default ..lena.bmp] \n");
		return;
	}
}

void MainWindow::CropAndScale(Mat mat, float scale, Rect newRect) {
	Mat trans= mat.clone();
	if (scale < 1.0f) {
		scale = 1.0f;
	}
	//resize(mat, trans, Size(WINDOW_WIDTH, WINDOW_HEIGHT), scale, scale, 1);

	roiRect = newRect;
	Mat roi= trans(roiRect);

	if (roiRect.x < 0) {
		roiRect.x = 0;
	}
	if (roiRect.y < 0) {
		roiRect.y = 0;
	}
	if (roiRect.x > trans.size().width - WINDOW_WIDTH - 1)
	{
		roiRect.x = trans.size().width - WINDOW_WIDTH - 1;
	}
	if (roiRect.y > trans.size().height - WINDOW_HEIGHT - 1)
	{
		roiRect.y = trans.size().height - WINDOW_HEIGHT - 1;
	}

	imshow(window_name, roi);
}

void MainWindow::OnMouse(int event, int x, int y, int flags, void* ustc) {
	Rect newRect= roiRect;
	if (event == EVENT_MOUSEMOVE) {
		if (!drag) {
			seed = Point(x, y);
			drag = true;
		}
		else {
			int dx = x - seed.x;
			int dy = y - seed.y;
			newRect = Rect(newRect.x + dx, newRect.y + dy, WINDOW_WIDTH, WINDOW_HEIGHT);
			CropAndScale(src, 1.0f, newRect);
		}
	}
	if (event == EVENT_MOUSEWHEEL) {
		float scale= 1+getMouseWheelDelta(flags)/1200.0f;
		CropAndScale(src, scale, newRect);
	
	}

}