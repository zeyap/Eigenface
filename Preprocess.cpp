#include "preprocess.h"

static Mat src;
static Mat display;
static Rect roiRect;
static Point seed;
static Mat anchors;
float scale;
bool alignCenter;
static Mat currdisplay;
static char* window_name;

Preprocess::Preprocess(char* newWindowName) {
	window_name = newWindowName;
	scale = 1.0f;
	setMouseCallback(window_name, OnMouse, 0);

	for (int i = 1; i <= MAX_IMAGE_NUMBER; i++) {
		if (Utility::FileExist("facedb/s" + to_string(i) + ".jpg") == false) {
			alignCenter = true;
			string fname = "faces/s" + to_string(i);
			if (i < MAX_IMAGE_NUMBER) {
				fname += ".pgm";
			}
			else {
				fname += ".jpg";
			}
			ReadImage(Utility::CVStringToChar(fname));
			currdisplay = CropAndScale(display, &scale, Rect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));
			int key = waitKey(0);
			if (key == 13) {
				SaveImage(currdisplay, i);
			}
		}
	}


}

void Preprocess::ReadImage(const char* filename) {
	src = imread(filename, IMREAD_COLOR);
	if (src.empty()) {
		printf(" Error opening image\n");
		return;
	}
	copyMakeBorder(src, display, (IMAGE_SIDE_LEN - src.size().height) / 2, (IMAGE_SIDE_LEN - src.size().height) / 2, (IMAGE_SIDE_LEN - src.size().width) / 2, (IMAGE_SIDE_LEN - src.size().width) / 2, BORDER_CONSTANT, Scalar(0, 0, 0));
}

Mat Preprocess::CropAndScale(Mat mat, float* scale, Rect newRect) {
	Mat newMat = mat.clone();
	roiRect = newRect;
	if (*scale < 0.8f) {
		*scale = 0.8f;
	}
	else if (*scale > 2.0f) {
		*scale = 2.0f;
	}
	roiRect.width = newRect.width / (*scale);
	roiRect.height = newRect.height / (*scale);

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
	Mat noAnchors = roi.clone();
	DisplayAnchors(roi);
	imshow(window_name, roi);
	return noAnchors;
}

void Preprocess::SaveImage(Mat roi, int i) {
	Size sz = Size(OUTPUT_W, OUTPUT_H);
	Rect saveRect = Rect(WINDOW_WIDTH*(0.5 - EYE_WIDTH_R) / scale, WINDOW_HEIGHT*(EYE_HEIGHT_R / 2) / scale, WINDOW_WIDTH*EYE_WIDTH_R*2.5 / scale, WINDOW_HEIGHT*7.0 / 12 / scale);
	Mat save_img = roi(saveRect);
	resize(save_img, save_img, sz);

	cvtColor(save_img, save_img, COLOR_BGR2GRAY);
	equalizeHist(save_img, save_img);

	if (save_img.empty())
	{
		std::cerr << "Something is wrong with the webcam, could not get frame." << std::endl;
	}

	imwrite(Utility::CVStringToChar("facedb/s" + to_string(i) + ".jpg"), save_img);

}

void Preprocess::OnMouse(int event, int x, int y, int flags, void* ustc) {
	Rect newRect = roiRect;
	seed = Point(newRect.width / 2, newRect.height / 2);
	int dx = x - seed.x;
	int dy = y - seed.y;
	if (event == EVENT_LBUTTONDOWN) {
		if (abs(dx)>WINDOW_WIDTH / 5) {
			newRect = Rect(newRect.x + dx*0.03f, newRect.y, WINDOW_WIDTH, WINDOW_HEIGHT);
		}
		if (abs(dy)>WINDOW_HEIGHT / 5) {
			newRect = Rect(newRect.x, newRect.y + dy*0.03f, WINDOW_WIDTH, WINDOW_HEIGHT);
		}
		cout << newRect.x << "/t" << newRect.y << "/n";
	}

	if (event == EVENT_MOUSEWHEEL) {
		scale += getMouseWheelDelta(flags) / 2000.0f;
		cout << newRect.x << "/t" << newRect.y << "/n";
	}

	if (event == EVENT_RBUTTONDOWN) {
		alignCenter = true;
	}

	newRect = Rect(newRect.x, newRect.y, WINDOW_WIDTH, WINDOW_HEIGHT);

	currdisplay = CropAndScale(display, &scale, newRect);

}

void Preprocess::InitAnchors(Mat & mat) {
	Scalar color = Scalar(255, 255, 255);
	anchors = Mat::zeros(mat.size(), mat.type());
	putText(anchors, "+", Point(mat.size().width *(0.5 - EYE_WIDTH_R / 2), mat.size().height *EYE_HEIGHT_R), FONT_HERSHEY_PLAIN, 2 / scale, color, 1, 8);
	putText(anchors, "+", Point(mat.size().width *(0.5 + EYE_WIDTH_R / 2), mat.size().height *EYE_HEIGHT_R), FONT_HERSHEY_PLAIN, 2 / scale, color, 1, 8);

	putText(anchors, "Press [Enter] to confirm", Point(mat.size().width *0.05, mat.size().height *0.05), FONT_HERSHEY_PLAIN, 1 / scale, color, 1, 8);
	putText(anchors, "Right click to center", Point(mat.size().width *0.05, mat.size().height *0.10), FONT_HERSHEY_PLAIN, 1 / scale, color, 1, 8);
	putText(anchors, "<", Point(mat.size().width *0.05, mat.size().height *0.50), FONT_HERSHEY_PLAIN, 3 / scale, color, 1, 8);
	putText(anchors, ">", Point(mat.size().width *0.95, mat.size().height *0.50), FONT_HERSHEY_PLAIN, 3 / scale, color, 1, 8);
	putText(anchors, "^", Point(mat.size().width *0.50, mat.size().height *0.05), FONT_HERSHEY_PLAIN, 3 / scale, color, 1, 8);
	putText(anchors, "v", Point(mat.size().width *0.50, mat.size().height *0.95), FONT_HERSHEY_PLAIN, 3 / scale, color, 1, 8);

}
void Preprocess::DisplayAnchors(Mat & mat) {
	InitAnchors(mat);
	addWeighted(mat, 1.0, anchors, 1.0, 0.0, mat);

}