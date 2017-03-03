#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include "CImageProcessingEngine.h"
#include "opencv2/opencv.hpp"

using namespace cv;

CImageProcessingEngine::CImageProcessingEngine() {

}

CImageProcessingEngine::~CImageProcessingEngine() {

}

cv::Mat& CImageProcessingEngine::FIND_REGION(cv::Mat &image, int x, int y) {
	Mat temp;
	return temp;
}
cv::Mat& CImageProcessingEngine::FIND_PERIMETER(cv::Mat &image, int x, int y) {
	Mat temp;
	return temp;
}
void CImageProcessingEngine::DISPLAY_IMAGE() {

}
void CImageProcessingEngine::DISPLAY_PIXELS() {

}
void CImageProcessingEngine::SAVE_PIXELS() {

}