#include "mainwindow.h"

MainWindow::MainWindow() {

	Preprocess preprocess;
	
	Train myTrain;
	
	int PCs[] = {10,25,50,100};
	for (int k = 0; k < 4; k++) {
		int hit = 0;
		int miss = 0;
		for (int i = 1; i < MAX_IMAGE_NUMBER; i++) {
			for (int j = 1; j < MAX_TEST_INDEX; j++) {
				string fname = "facedb/s" + to_string(i) + "_" + to_string(j+5) + ".jpg";
				Test myTest(fname, PCs[k]);
				if (myTest.closestCandidate == i) {
					hit++;
				}
				else {
					miss++;
				}
			}
		}
		cout << "PC = " << PCs[k] << endl;
		cout << "miss = " << miss << endl;
		cout << "hit = " << hit << endl;
		cout << endl;
	}
	
	Reconstruct myConstruct("facedb/s41.jpg",50);
 	waitKey();
	
}
