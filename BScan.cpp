#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
//C++
#include <iostream>
#include <sstream>
#include <cstring>

//local
#include "BScan.h"

using namespace std;
using namespace cv;

float minRatio = 0.7f;
int dimx = 15;
int dimy = 15;

BScan::BScan(Mat f) {
	// copy f to local frame var
	frame = f.clone();

	//copy frame to mask
	mask = frame.clone();
}

Mat BScan::scanIt() {
	//each block to be 16x16 pixels
	cv::Size block(dimx, dimy);

	for(int j = 0; j < frame.rows; j += block.height) {
		for(int i = 0; i < frame.cols; i += block.width) {
			//CURRENT BLOCK
			Rect currentBlock(i, j, block.width, block.height);

			unsigned int cWhite = 0, cBlack = 0;

			//loop through the currentBlock object and add up all black and white pixels in it
			for(int y = currentBlock.y; y < currentBlock.y + currentBlock.height; ++y) {
				for(int x = currentBlock.x; x < currentBlock.x + currentBlock.width; ++x) {
					if(y < frame.rows && x < frame.cols) {
						if(frame.at<unsigned char>(y,x) == 255) {
							cWhite++;
						} else {
							cBlack++;
						}
					}
				}
			}

			unsigned char blockColor = 0;

			//if the ratio is greater than or equal to the minimum ratio value, we make the block white.
			//else leave black.
			if((float)cWhite / ((float)cWhite + (float)cBlack) > minRatio) blockColor = 255;


			//this loops through all pixels the area of the current block; sets the pixel color accoring to
			//the blockColor value
			for(int y = currentBlock.y; y < currentBlock.y + currentBlock.height; ++y) {
				for(int x = currentBlock.x; x < currentBlock.x + currentBlock.width; ++x) {
					if(y < frame.rows && x < frame.cols) {
						mask.at<unsigned char>(y,x) = blockColor;
						if(blockColor == 255) {
							x = x, y = y;
						}
					}
				}
			}
		}
	}

	return mask;
}