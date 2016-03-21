//OpenCV
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
// C++
#include <iostream>
#include <sstream>
#include <unistd.h>

// Local
#include "BScan.h"
#include "Estimator.h"
#include "Coords.h"

using namespace std;
using namespace cv;

class Main {
public:
	Main();
	~Main();
	void processFeed(void);

private:
	Mat frame, fgMaskMOG2, mask;

	const int CAMERA = 0;

	Ptr<BackgroundSubtractorMOG2> pMOG2;
	int keyboard;
};

Main::Main() {
	//namedWindow("Normal", 1);
	//namedWindow("Motion Tracking", 1);
	pMOG2 = createBackgroundSubtractorMOG2();
}

Main::~Main() {
	// Do Nothing
}

void Main::processFeed(void) {
	VideoCapture capture;
	capture.open(CAMERA);
	if(!capture.isOpened()) {
		cerr << "Unable to open video feed." << endl;
		exit(EXIT_FAILURE);
	}

	while((char)keyboard != 'q' && (char)keyboard != 27) {
		if(!capture.read(frame)) {
			cerr << "Unable to read next frame." << endl;
            cerr << "Exiting..." << endl;
            exit(EXIT_FAILURE);
		}

		pMOG2->apply(frame, fgMaskMOG2);

		BScan scanner(fgMaskMOG2);

		mask = scanner.scanIt();
		
		//rectangle(mask, cv::Point(10, 2), cv::Point(100,20), cv::Scalar(255,255,255), -1);
		//string recStr = to_string(*horiz) + ", " + to_string(*vert);
		//putText(mask, recStr, cv::Point(15, 15), FONT_HERSHEY_SIMPLEX, 0.5 , cv::Scalar(0,0,0));

		Estimator* estimator = new Estimator();

		Coords coords = estimator->estimateTarget(mask);
		
		int row = coords.row;
		int col = coords.col;

		rectangle(frame, Point((col - 1) * BScan::DIM_X, (row - 1) * BScan::DIM_Y), Point(col * BScan::DIM_X, row * BScan::DIM_Y), Scalar(0, 0, 255), -1);

		imshow("Normal", frame);
		//imshow("Motion Tracking", mask);

		delete estimator;
		estimator = NULL;
		keyboard = waitKey(30);
	}
	capture.release();
}

int main(int argc, char** argv) {
	Main* main = new Main();
	main->processFeed();

	cout << "Exiting..." << endl;
	delete main;
	return 0;
}