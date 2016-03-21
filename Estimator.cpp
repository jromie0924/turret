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
			Scalar color = frame.at<unsigned char>(row, col);
			if(color.val[0] != 0) {
				matrix[nextRow][nextCol++] = 1;
			} else {
				matrix[nextRow][nextCol++] = 0;
			}
		}
		nextRow++;
		nextCol = 0;
	}

/*
	// Cool binary representation of motion detector output in the terminal
	for(int row = 0; row < ROWS; row++) {
		for(int col = 0; col < COLS; col++) {
			cout << matrix[row][col];
		}
		cout << endl;
	}
*/

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

	int max = 0;

	// Get the maximum value in the col array
	for(int a = 0; a < COLS; a++) {
		if(colScanMat[a] > max) {
			max = colScanMat[a];
		}
	}

	// Get the max count
	max = 0;
	for(int a = 0; a < COLS; a++) {
		if(rowScanMat[a] > max) {
			max = colScanMat[a];
		}
	}

	int* pos = new int[1];
	int next = 0;
	for(int a = 0; a < COLS; a++) {
		if(colScanMat[a] == max) {
			pos[next++] = a;
		}
	}

	int sum = 0;
	for(int a = 0; a < next; a++) {
		sum += colScanMat[pos[a]];
	}

	delete[] pos;
	pos = NULL;

	int colAvg = sum / next;


	// Do the same for rows
	for(int row = 0; row < ROWS; row++) {
		for(int col = 0; col < COLS; col++) {
			if(matrix[row][col] == 1) {
				rowScanMat[col]++;
			}
		}
	}

	// Get the max count for the row count array
	max = 0;
	for(int a = 0; a < ROWS; a++) {
		if(rowScanMat[a] > max) {
			max = rowScanMat[a];
		}
	}

	// Get positions of all occurances of the max value
	pos = new int[1];
	next = 0;
	for(int a = 0; a < ROWS; a++) {
		if(rowScanMat[a] == max) {
			pos[next++] = a;
		}
	}

	// Get the average position
	sum = 0;
	for(int a = 0; a < next; a++) {
		sum += rowScanMat[pos[a]];
	}

	delete[] pos;
	pos = NULL;

	int rowAvg = sum / next;

	coords.setCoords(rowAvg, colAvg);

	// Delete the 2D array
	for(int i = 0; i < ROWS; i++) {
		delete[] matrix[i];
		matrix[i] = NULL;
	}
	delete[] matrix;
	matrix = NULL;

	// Return the coordinates
	return coords;
}

/*
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

	//waitKey(0);

	return 0;
}*/