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
#include "TScan.h"

using namespace std;
using namespace cv;

float minRatio = 0.8f;

TScan::TScan(Mat f) {
	// copy f to local frame var
	frame = f.clone() > 0;

	//copy frame to mask
	mask = frame.clone();

	//This makes the background of the masked matrix to a grey color
	//Makes it so we can see all individual blocks.
	mask = 100;
}

Mat* TScan::scanIt() {
	//each block to be 16x16 pixels
	cv::Size block(16, 16);

	for(int j = 0; j < frame.rows; j += block.height) {
		for(int i = 0; i < frame.cols; i += block.width) {
			//CURRENT BLOCK
			Rect currentBlock(i, j, block.width, block.height);

			unsigned int cWhite = 0, cBlack = 0;

			//loop through the current block and add up all black and white pixels in it
			for(int y = currentBlock.y + 1; y < currentBlock.y + currentBlock.height - 1; ++y) {
				for(int x = currentBlock.x + 1; x < currentBlock.x + currentBlock.width - 1; ++x) {
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
			for(int y = currentBlock.y + 1; y < currentBlock.y + currentBlock.height - 1; ++y) {
				for(int x = currentBlock.x + 1; x < currentBlock.x + currentBlock.width - 1; ++x) {
					if(y < frame.rows && x < frame.cols) {
						mask.at<unsigned char>(y,x) = blockColor;
						if(blockColor == 255) {
							xVal = x, yVal = y;
						}
					}
				}
			}
		}
	}

	return &mask;
}