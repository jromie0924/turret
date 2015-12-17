#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
//C++
#include <iostream>
#include <sstream>
#include <thread>
#include <mutex>

//local
#include "TScan.h"

using namespace std;
using namespace cv;

Size block(16, 26);

TScan::TScan(Mat f) {
	frame = f.clone();
	thres = frame > 0;
	mask = thres.clone();
	mask = 100;
	minRatio = 0.3f;
}

Mat& TScan::scanIt() {

	


	return frame;
}