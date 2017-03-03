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

	void FIND_REGION(cv::Mat &srcimage, cv::Mat &dstimage, int x, int y);
	cv::Mat& FIND_PERIMETER(cv::Mat &image, int x, int y);
	void DISPLAY_IMAGE(const cv::Mat &image, std::string const &win_name);
	void DISPLAY_PIXELS();
	void SAVE_PIXELS();

private:
	void show_mat(const cv::Mat &image, std::string const &win_name);
	
	bool FIND_REGION_isSimilar(Vec3b *p1, Vec3b *p2);
}; 

#endif
