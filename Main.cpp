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
#include <deque>
#include <cstring>

// Local
#include "BScan.h"
#include "Estimator.h"
#include "Coords.h"
#include "Comm.h"

#define MAX_LEN 5

using namespace std;
using namespace cv;

class Main {
public:
	Main();
	~Main();
	void processFeed(void);
	int colAvg(void);
	int rowAvg(void);

private:
	Mat frame, fgMaskMOG2, mask;

	const int CAMERA = 0;

	Ptr<BackgroundSubtractorMOG2> pMOG2;
	int keyboard;
	deque<int> colCache;
	deque<int> rowCache;
};

Main::Main() {
	pMOG2 = createBackgroundSubtractorMOG2();
}

Main::~Main() {
	// Do Nothing
}

int Main::colAvg(void) {
	int total = 0;
	for(int a = 0; a < colCache.size(); a++) {
		total += colCache.at(a);
	}
	return (total / colCache.size());
}

int Main::rowAvg(void) {
	int total = 0;
	for(int a = 0; a < rowCache.size(); a++) {
		total += rowCache.at(a);
	}
	return (total / rowCache.size());
}

void Main::processFeed(void) {
	VideoCapture capture;
	capture.open(CAMERA);
	if(!capture.isOpened()) {
		cerr << "Unable to open video feed." << endl;
		exit(EXIT_FAILURE);
	}

	int counter = 0;
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

		Estimator estimator;

		Coords coords = estimator.estimateTarget(mask);

		if(counter < MAX_LEN) {
			rowCache.push_front(coords.row);
			colCache.push_front(coords.col);
		} else {
			rowCache.pop_back();
			colCache.pop_back();
			rowCache.push_front(coords.row);
			colCache.push_front(coords.col);
		}

		int row = rowAvg();
		int col = colAvg();

		rectangle(frame, Point(col * BScan::DIM_X, row * BScan::DIM_Y), Point((col + 1) * BScan::DIM_X, (row + 1) * BScan::DIM_Y), Scalar(0, 0, 255), -1);

		// Generate the data
		stringstream convertRow;
		stringstream convertCol;
		convertRow << to_string(row);
		convertRow << ",";
		convertCol << to_string(col);
		convertRow << convertCol.str();
		string data = convertRow.str();

		// Send the data!
		Comm comm;
		comm.sendData(data.c_str());

		namedWindow("Press Q or Esc to exit", 1);
		//namedWindow("Motion Tracking", 1);
		imshow("Press Q orEsc to exit", frame);
		//imshow("Motion Tracking", mask);
		if(counter < MAX_LEN) {
			counter++;
		}
		keyboard = waitKey(30);
	}
	capture.release();
}

int main(int argc, char** argv) {
	Main* main = new Main();
	main->processFeed();

	cout << "Exiting..." << endl;
	delete main;
	main = NULL;
	return 0;
}