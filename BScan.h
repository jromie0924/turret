#ifndef BSCAN_H
#define BSCAN_H

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
	//original matrix
	Mat frame;

	//matrix to be modified
	Mat mask;

public:
	//integer values to keep track of the location of white blocks
	int xVal, yVal;
	BScan(Mat);

	//scanning function returns a pointer to the masked matrix
	Mat* scanIt();
};



#endif