#ifndef CImageProcessingEngine_h
#define CImageProcessingEngine_h

#include <stdio.h>
#include "opencv2/opencv.hpp"
using namespace cv;

/**
* CImageProcessingEngine - 
*/
class  CImageProcessingEngine 
{
public:

	CImageProcessingEngine();
	~CImageProcessingEngine();

	cv::Mat& FIND_REGION(cv::Mat &image, int x, int y);
	cv::Mat& FIND_PERIMETER(cv::Mat &image, int x, int y);
	void DISPLAY_IMAGE(const cv::Mat &image, std::string const &win_name);
	void DISPLAY_PIXELS();
	void SAVE_PIXELS();

private:
	void show_mat(const cv::Mat &image, std::string const &win_name);
}; 

#endif
