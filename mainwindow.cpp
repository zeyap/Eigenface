#include "mainwindow.h"

char window_name[] = "Eigenface";

MainWindow::MainWindow() {

	namedWindow(window_name, WINDOW_NORMAL|WINDOW_KEEPRATIO);
	resizeWindow(window_name, Size(WINDOW_WIDTH, WINDOW_HEIGHT));
	Preprocess preprocess(window_name);
	destroyWindow(window_name);

	Train train;

	Test test("facedb/s5.jpg", 25);

 	waitKey();
	
}
