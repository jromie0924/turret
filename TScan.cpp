#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
//C++
#include <iostream>
#include <sstream>
//#include <thread>
//#include <mutex>

//local
#include "TScan.h"

using namespace std;
using namespace cv;

cv::Size block(16, 26);

TScan::TScan(Mat f) {
	frame = f.clone();
	thres = frame > 0;
	mask = thres.clone();
	mask = 100;
	minRatio = 0.4f;
}

Mat TScan::scanIt() {
	for(int j = 0; j < frame.rows; j += block.height) {
		for(int i = 0; i < frame.cols; i += block.width) {
			//CURRENT BLOCK
			Rect currentBlock(i, j, block.width, block.height);

			unsigned int cWhite = 0, cBlack = 0;

			for(int y = currentBlock.y + 1; y < currentBlock.y + currentBlock.height; ++y) {
				for(int x = currentBlock.x + 1; x < currentBlock.x + currentBlock.width; ++x) {
					if(y < frame.rows && x < frame.cols) {
						if(thres.at<unsigned char>(x,y) == 255) {
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
						mask.at<unsigned char>(x,y) = blockColor;
					}
				}
			}
		}
	}

	return &mask;
}