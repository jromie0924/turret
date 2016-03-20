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
#include "BScan.h"

using namespace std;
using namespace cv;

int** Estimator::convertToMatrix(Mat& frame) {
	// Rows x Columns
	static int matrix[(unsigned int)BScan.ROWS][(unsigned int)BScan.COLS];

	int nextRow = 0;
	int nextCol = 0;
	for(int row = 0; row < frame.rows; row += BScan.ROWS) {
		for(int col = 0; col < frame.cols; col += BScan.COLS) {
			if(frame.at<unsigned char>(row, col) == 255) {
				matrix[nextRow][nextCol++] = 1;
			} else {
				matrix[nextRow][nextCol++] = 0;
			}
		}
		nextRow++;
		nextCol = 0;
	}
	return matrix;
}

static Coords Estimator::estimateTarget(Mat& frame) {
	Coords coords;
	int** matrix = convertToMatrix(Mat);
	int colScanMat[BScan.COLS];
	int rowScanMat[BScan.ROWS];

	// Scan the columns of the matrix
	for(int col = 0; col < BScan.COLS; col++) {
		for(int row = 0; row < BScan.ROWS; row++) {
			if(matrix[col][row] == 1) {
				colScanMat[col]++;
			}
		}
	}

	// Find the region of the column scan for range of the row scan
	int threshold = 1;
	int max = 0;

	// Get the maximum value in the array
	for(int a = 0; a < BScan.COLS; a++) {
		if(colScanMat[a] > max) {
			max = colScanMat[a];
		}
	}

	// Use the threshold to determinie an estimated best group of (adjacent) columns to scan
	int streakMax = 0;
	int streak = 0;
	int low;
	int high;
	for(int a = 0; a < BScan.COLS; a++) {
		int val = colScanMat[a];
		if(val == max || val == max + threshold || val == max - threahold) {
			if(streak == 0) {
				low = a;
			}
			streak ++;			
		} else {
			high = a;
			streakMax = streak;
			streak = 0
		}
	}

}