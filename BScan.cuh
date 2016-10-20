#pragma once

//opencv
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
//C++
#include <iostream>
#include <sstream>

using namespace std;
using namespace cv;

class BScan {
private:
	// Original matrix
	Mat frame;

	// Matrix to be modified
	Mat mask;

public:
	//integer values to keep track of the location of white blocks
	//int x, y;
	BScan(Mat);

	//scanning function returns a pointer to the masked matrix
	Mat scanIt();
	cv::Size getDims();

	//static const int ROWS;
	//static const int COLS;
	//static const int DIM_Y;
};
