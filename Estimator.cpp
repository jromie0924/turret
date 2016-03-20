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

int** Estimator::convertToMatrix(Mat& frame) {
	// Rows x Columns
	static int matrix[(unsigned int)BScan.DIM_Y][(unsigned int)BScan.DIM_X];

	for(int row = 0; row < frame.rows; row += DIM_Y) {
		for(int col = 0; col < frame.cols; col += DIM_X) {
			if(frame.at<unsigned char>(row, col) == 255) {
				matrix[row][col] = 1;
			} else {
				matrix[row][col] = 0;
			}
		}
	}
	return matrix;
}

static Coords Estimator::estimateTarget(Mat& frame) {
	Coords coords;
	
}