#include "mainwindow.h"

char window_name[] = "Eigenface";

MainWindow::MainWindow() {

	namedWindow(window_name, WINDOW_NORMAL|WINDOW_KEEPRATIO);
	resizeWindow(window_name, Size(WINDOW_WIDTH, WINDOW_HEIGHT));

	Train train;
	Preprocess preprocess(window_name);
	
}

