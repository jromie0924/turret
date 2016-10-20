// OpenCV
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>

// C
#include <stdio.h>

// Local
#include "BScan.cuh"

const float MIN_RATIO = 0.5f;
const int TILE_SIZE = 16;

BScan::BScan(Mat f) {
	// copy f to local frame var
	frame = f.clone();

	//copy frame to mask
	mask = frame.clone();
}

__global__ void scan_kernel(unsigned char* g_img) {
  int i = threadIdx.x + blockIdx.x * blockDim.x;
  int j = threadIdx.y + blockIdx.y * blockDim.y;

  int white = 0;
  int black = 0;
  __shared__ unsigned char blockColor;
  __shared__ int count_white;
  __shared__ int count_black;
  __shared__ unsigned char s_tile[TILE_SIZE * TILE_SIZE];
  blockColor = 0;
  count_white = 0;
  count_black = 0;

  // Copy to shared memory
  int s_pos = threadIdx.x + threadIdx.y * blockDim.y;
  s_tile[s_pos] = g_img[i + j * TILE_SIZE];

  //__syncthreads();

  // Operate on shared memory
  if(s_tile[s_pos] == 255) {
    white = 1;
  } else {
    black = 1;
  }

  __syncthreads();

  atomicAdd(&count_white, white);
  atomicAdd(&count_black, black);

  __syncthreads();

  if(threadIdx.x == 0) {
    if((float)count_white / ((float)count_white + (float)count_black) > MIN_RATIO) {
      blockColor = 255;
    }
  }

  __syncthreads();

  s_tile[s_pos] = blockColor;

  g_img[i + j * TILE_SIZE] = s_tile[s_pos];
}

// Set up and call the GPU kernel
cv::Mat BScan::scanIt() {
  unsigned char* h_frame = frame.data;

  int numPixels = frame.total();
  int numRows = frame.rows;
  int numCols = frame.cols;

  unsigned char* d_frame;
  cudaMalloc(&d_frame, numPixels * sizeof(unsigned char));
  cudaMemcpy(d_frame, h_frame, numPixels * sizeof(unsigned char), cudaMemcpyHostToDevice);

  dim3 blocks_tiled(numCols / TILE_SIZE, numRows / TILE_SIZE);
  dim3 threads_tiled(TILE_SIZE, TILE_SIZE);
  scan_kernel<<<blocks_tiled, threads_tiled>>>(d_frame);

  unsigned char* output = (unsigned char*) malloc(numPixels * sizeof(unsigned char));

  cudaMemcpy(output, d_frame, numPixels * sizeof(unsigned char), cudaMemcpyDeviceToHost);

  cv::Mat retMat = cv::Mat(numRows, numCols, CV_8UC1, output);

  cudaFree(d_frame);
  free(output);

  return retMat;
}

cv::Size BScan::getDims() {
  int rows = frame.rows / TILE_SIZE;
  int cols = frame.cols / TILE_SIZE;

  cv::Size sz;
  sz.height = rows;
  sz.width = cols;

  return sz;
}
