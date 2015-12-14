#include "opencv2/objdetect.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

int getFeed(VideoCapture vc) {
	if(vc.isOpened()) {
		Mat frame;
		if(!vc.read(frame))
			return -1;
	} else {
		return 0;
	}
}

int capVideo(VideoCapture *vc, Mat *cameraFrame) {
	while(1) {
		vc->read(*cameraFrame);
		imshow("Camera output", *cameraFrame);
		if(waitKey(30) >= 0) break;
	}
	return 0;
}

int main() {
	VideoCapture cap;
	Mat frame;
	cap.open(0);
	if(!cap.isOpened()) {
		cout << "cannot open camera" << endl;
		return -1;
	}

	Mat buffer[2];
	for(int a = 0; a < 2; a++) {
		cap.read(frame);
		buffer[a] = frame;
	}
	int retVal = capVideo(&cap, &frame);
	return 0;
}