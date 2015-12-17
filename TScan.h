#ifndef TSCAN_H
#define TSCAN_H

//opencv
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

using namespace std;
using namespace cv;

class TScan {
private:
	Mat frame;
	Mat thres;
	Mat mask;
	float minRatio;

public:
	TScan(Mat);
	Mat& scanIt();
};



#endif