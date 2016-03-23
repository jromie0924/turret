//opencv
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>

//C++
#include <iostream>
#include <sstream>
#include <vector>

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
	const int ROWS = BScan::ROWS;
	const int COLS = BScan::COLS;

	int** matrix = new int*[ROWS];
	for(int i = 0; i < ROWS; i++) {
		matrix[i] = new int[COLS];
	}

	Coords coords;
	convertToMatrix(frame, matrix);
	//int colScanMat[COLS];
	//int rowScanMat[ROWS];
	vector<int> colScanMat (COLS);
	vector<int> rowScanMat (ROWS);
	int sum;
	int max;
	int next = 0;
	vector<int> pos;


// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// COLUMN ESTIMATION
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	// Scan the columns of the matrix
	for(int col = 0; col < COLS; col++) {
		for(int row = 0; row < ROWS; row++) {
			if(matrix[row][col] == 1) {
				colScanMat.at(col)++;
			}
		}
	}

	// Get the maximum value in the col array
	max = 0;
	for(int a = 0; a < COLS; a++) {
		if(colScanMat.at(a) > max) {
			max = colScanMat.at(a);
		}
	}

	// Retrieve all locations of maximum value
	for(int a = 0; a < COLS; a++) {
		if(colScanMat.at(a) == max) {
			pos.push_back(a);
			next++;
		}
	}

	// Average the coordinate values
	sum = 0;
	for(int a = 0; a < next; a++) {
		sum += colScanMat.at(pos.at(a));
	}

	int colAvg = sum / next;

	pos.clear();

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ROW ESTIMATION
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	// Do the same for rows
	for(int row = 0; row < ROWS; row++) {
		for(int col = 0; col < COLS; col++) {
			if(matrix[row][col] == 1) {
				rowScanMat.at(row)++;
			}
		}
	}

	// Get the max count for the row count array
	max = 0;
	for(int a = 0; a < ROWS; a++) {
		if(rowScanMat.at(a) > max) {
			max = rowScanMat.at(a);
		}
	}

	// Get positions of all occurances of the max value
	next = 0;
	for(int a = 0; a < ROWS; a++) {
		if(rowScanMat.at(a) == max) {
			pos.push_back(a);
			next++;
		}
	}

	// Get the average position
	sum = 0;
	for(int a = 0; a < next; a++) {
		sum += rowScanMat.at(pos.at(a));
	}

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