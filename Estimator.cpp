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
#include <math.h>

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

void Estimator::convertToMatrix(Mat& frame, int** matrix, vector<Coords>& hits) {

	// Rows x Columns
	const unsigned int ROWS = BScan::ROWS;
	const unsigned int COLS = BScan::COLS;
	int nextRow = 0;
	int nextCol = 0;
	for(int row = 0; row < frame.rows; row += BScan::DIM_Y) {
		for(int col = 0; col < frame.cols; col += BScan::DIM_X) {
			Scalar color = frame.at<unsigned char>(row, col);
			if(color.val[0] != 0) {
				hits.push_back(Coords(nextRow, nextCol));
				matrix[nextRow][nextCol++] = 1;
			} else {
				matrix[nextRow][nextCol++] = 0;
			}
		}
		nextRow++;
		nextCol = 0;
	}

	numMovement = hits.size();

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

// Calculates line of best fit in the matrix (treating it as a grid).
// Then it iterates through the rows and finds the row which contains the most
// movement. That horizontal line (y = c) will intersect with the regression line
// at the point where the target is to be estimated.
Coords Estimator::estimateTarget(Mat& frame) {

	// Rows x Columns
	const int ROWS = BScan::ROWS;
	const int COLS = BScan::COLS;

	int** matrix = new int*[ROWS];
	for(int i = 0; i < ROWS; i++) {
		matrix[i] = new int[COLS];
	}

	Coords coords;
	vector<Coords> movement;
	convertToMatrix(frame, matrix, movement);

	double xTotal = 0;
	double yTotal = 0;
	double xyTotal = 0;
	double xSquaredTotal = 0;
	double xMean;
	double yMean;
	double xSquaredMean;
	double xMeanSquared;
	double xyMean;
	double b;
	double m;

	for(int a = 0; a < numMovement; a++) {
		Coords i = movement.at(a);
		xTotal += i.col;
		yTotal += i.row;
		xSquaredTotal += (double)(pow(i.col, 2));
		xyTotal += (i.col * i.row);
	}

	xMean = xTotal / numMovement;
	yMean = yTotal / numMovement;
	xSquaredMean = xSquaredTotal / numMovement;
	xyMean = xyTotal / numMovement;
	xMeanSquared = (double)(pow(xMean, 2));

	m = (xMean * yMean - xyMean) / (xMeanSquared - xSquaredMean);
	b = yMean - (m * xMean);

	//---------------------------------------------
	// Now we have a linear equation y = mx + b
	//---------------------------------------------

	struct Pair {
		int actionHits;
		//int row;
	};

	// Iterate through rows
	struct Pair maxHits;
	vector<int> rows;
	int maxCount = 0;
	maxHits.actionHits = 0;
	//maxHits.row = 0;
	for(int r = 0; r < ROWS; r++) {
		int count = 0;
		for(int c = 0; c < COLS; c++) {
			if(matrix[r][c] == 1) {
				count++;
			}
		}
		if(count > maxHits.actionHits) {
			maxHits.actionHits = count;
			rows.clear();
			rows.push_back(r);
			//maxHits.row = r;
		} else if(count == maxHits.actionHits) {
			maxCount++;
			rows.push_back(r);
		}
	}

	int c;

	if(rows.size() > 1) {
		int rowSum = 0;
		for(int a = 0; a < rows.size(); a++) {
			rowSum += rows.at(a);
		}
		c = rowSum / rows.size();
	} else {
		c = rows.at(0);
	}

	int x = floor((c - b) / m);

	coords.setCoords(c, x);

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