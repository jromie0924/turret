#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
//C++
#include <iostream>
#include <sstream>
#include <cstring>
//#include <thread>
//#include <mutex>

//local
#include "TScan.h"

using namespace std;
using namespace cv;

float minRatio = 0.3f;

TScan::TScan(Mat f) {
	frame = f.clone();
	//frame = frame > 0;
	mask = frame.clone();
	mask = 100;
}

Mat* TScan::scanIt() {
	cv::Size block(16, 16);

	for(int j = 0; j < frame.rows; j += block.height) {
		for(int i = 0; i < frame.cols; i += block.width) {
			//CURRENT BLOCK
			Rect currentBlock(i, j, block.width, block.height);

			unsigned int cWhite = 0, cBlack = 0;

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
			if((float)cWhite / ((float)cWhite + (float)cBlack) > minRatio) blockColor = 255;

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