// OpenCV
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>

// C
#include <stdio.h>

const float MIN_RATIO = 0.5f;
const int BScan::TILE_SIZE = 16;

__global__ void scan_kernel(unsigned char* g_img, int size, int t_size) {
  int i = threadIdx.x + blockIdx.x * blockDim.x;
  int j = threadidx.y + blockIdx.y * blockDim.y;

  int int white = 0;
  int black = 0;
  __shared__ unsigned char blockColor = 0;
  __shared__ int count_white;
  __shared__ int count_black;

  // Shared memory allocation
  __shared__ unsigned char* s_tile[t_size * t_size];

  // Copy to shared memory
  int s_pos = threadIdx.x + threadIdx.y * blockDim.y;
  s_tile[s_pose] = g_img[i + j * size];

  //__syncthreads();

  // Operate on shared memory
  if(s_tile[s_pose] == 255) {
    white = 1;
  } else {
    black = 1;
  }

  __syncthreads();

  atomicAdd(&count_white, white);
  atomicAdd(&count_black, black);

  __syncthreads();

  if(threadIdx.x == 0) {
    if((float)count_white + (float)count_white + (float)count_black > MIN_RATIO) {
      blockColor = 255;
    }
  }

  __syncthreads();

  s_tile[s_pose] = blockColor;

  g_img[i + j * t_size] = s_tile[s_pose];
}

// Set up and call the GPU kernel
cv::Mat scanIt(cv::Mat* frame) {
  unsigned char* h_frame = *frame.data;

  int numPixels = *frame.total()
  int numRows = *frame.rows;
  int numCols = *frame.cols;

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
