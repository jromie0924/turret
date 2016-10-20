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

//Local
#include "Coords.h"

using namespace std;
using namespace cv;

class Estimator {
public:
	Estimator(int, int, int);
	~Estimator();
	Coords estimateTarget(Mat&);

private:
	void convertToMatrix(Mat&, int**, vector<Coords>&);
	int rows;
	int cols;
	const int X_MAX = 43;
	const int Y_MAX = 32;
	int block_size;

	int numMovement;
};
