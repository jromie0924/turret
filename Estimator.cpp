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
#include "Estimator.h"
#include "Coords.h"
#include "BScan.h"

using namespace std;
using namespace cv;

Estimator::Estimator() {
	// Do Nothing
}

Estimator::~Estimator(){
	// Do nothing
}

void Estimator::convertToMatrix(Mat& frame, int** matrix) {

	// Rows x Columns
	const unsigned int ROWS = BScan::ROWS;
	const unsigned int COLS = BScan::COLS;
	int nextRow = 0;
	int nextCol = 0;
	for(int row = 0; row < frame.rows; row += BScan::DIM_Y) {
		for(int col = 0; col < frame.cols; col += BScan::DIM_X) {
			if(frame.at<unsigned char>(row, col) != 0) {
				matrix[nextRow][nextCol++] = 1;
			} else {
				matrix[nextRow][nextCol++] = 0;
			}
		}
		nextRow++;
		nextCol = 0;
	}
}

Coords Estimator::estimateTarget(Mat& frame) {

	// Rows x Columns
	const unsigned int ROWS = BScan::ROWS;
	const unsigned int COLS = BScan::COLS;

	int** matrix = new int*[ROWS];
	for(int i = 0; i < ROWS; i++) {
		matrix[i] = new int[COLS];
	}

	Coords coords;
	convertToMatrix(frame, matrix);
	int colScanMat[COLS];
	int rowScanMat[ROWS];

	// Scan the columns of the matrix
	for(int col = 0; col < COLS; col++) {
		for(int row = 0; row < ROWS; row++) {
			if(matrix[row][col] == 1) {
				colScanMat[col]++;
			}
		}
	}

	// Find the region of the column scan for range of the row scan
	int threshold = 0;
	int max = 0;

	// Get the maximum value in the array
	for(int a = 0; a < COLS; a++) {
		if(colScanMat[a] > max) {
			max = colScanMat[a];
		}
	}

	// Use the threshold to determinie an estimated best group of (adjacent) columns to scan
	int streakMax = 0;
	int streak = 0;
	int tempLow;
	int tempHigh;
	int colLow;
	int colHigh;
	for(int a = 0; a < COLS; a++) {
		int val = colScanMat[a];
		if(val == max || val == max + threshold || val == max - threshold) {
			if(streak == 0) {
				tempLow = a;
			}
			streak++;			
		} else {
			tempHigh = a;
			if(streak > streakMax) {
				streakMax = streak;
				colLow = tempLow;
				colHigh = tempHigh;
			}
			streak = 0;
		}
	}

	// Scan the region for hits
	for(int row = 0; row < ROWS; row++) {
		for(int col = colLow; col <= colHigh; col++) {
			if(matrix[row][col] == 1) {
				rowScanMat[row]++;
			}
		}
	}

	max = 0;
	for(int a = 0; a < ROWS; a++) {
		if(rowScanMat[a] > max) {
			max = rowScanMat[a];
		}
	}

	// Scan the row scan array using threshold just like for columns
	streakMax = 0;
	streak = 0;
	tempLow = 0;
	tempHigh = 0;
	int rowLow;
	int rowHigh;
	for(int a = 0; a < ROWS; a++) {
		int val = rowScanMat[a];
		if(val == max || val == max + threshold || val == max - threshold) {
			if(streak == 0) {
				tempLow = a;
			}
			streak++;
		} else {
			tempHigh = a;
			if(streak > streakMax) {
				streakMax = streak;
				rowLow = tempLow;
				rowHigh = tempHigh;
			}
			streak = 0;
		}
	}

	// Compute average values
	int rowAvg;
	int count = 0;
	int total = 0;
	for(int a = rowLow; a <= rowHigh; a++) {
		total++;
		count++;
	}

	rowAvg = total / count + 1;

	int colAvg = 0;
	count = 0;
	total = 0;
	for(int a = colLow; a <= colHigh; a++) {
		total++;
		count++;
	}

	colAvg = total / count + 1;

	coords.setCoords(32, 43);

	for(int row = 0; row < ROWS; row++) {
		for(int col = 0; col < COLS; col++) {
			cout << matrix[row][col];
		}
		cout << endl;
	}

	// Delete the 2D array
	for(int i = 0; i < ROWS; i++) {
		delete[] matrix[i];
	}
	delete[] matrix;

	return coords;
}

int main() {
	Mat image;
	image = imread("screenshots/testImage1.png", CV_LOAD_IMAGE_COLOR);
	if(!image.data) {
		cerr << "Could not load image\n";
		return -1;
	}

	Estimator *estimator = new Estimator();
	Coords coords = estimator->estimateTarget(image);

	cout << "Row: " << coords.row << endl;
	cout << "Col: " << coords.col << endl;

	delete estimator;

	int row = coords.row;
	int col = coords.col;

	rectangle(image, Point((col - 1) * BScan::DIM_X, (row - 1) * BScan::DIM_Y), Point(col * BScan::DIM_X, row * BScan::DIM_Y), Scalar(0, 0, 255), -1);

	//namedWindow("Display", 1);
	//imshow("Display", image);

	waitKey(0);

	return 0;
}