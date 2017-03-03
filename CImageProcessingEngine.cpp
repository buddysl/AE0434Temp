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
void CImageProcessingEngine::DISPLAY_IMAGE(const cv::Mat &image, std::string const &win_name) {
	//Mat image;
	//image = imread(filename, 1);
	//if (!image.data) {
	//	printf("No image data (warning: OpenCV recognize files by extensions)\n");
	//	return;
	//}
	//show_mat(image, "DISPLAY_IMAGE");

	namedWindow(win_name, WINDOW_AUTOSIZE);
	imshow(win_name, image);
	waitKey(0);
}

void CImageProcessingEngine::DISPLAY_PIXELS() {

}
void CImageProcessingEngine::SAVE_PIXELS() {

}

// show matrix
void CImageProcessingEngine::show_mat(const cv::Mat &image, std::string const &win_name) {
	namedWindow(win_name, WINDOW_AUTOSIZE);
	imshow(win_name, image);
	//waitKey(0);
}