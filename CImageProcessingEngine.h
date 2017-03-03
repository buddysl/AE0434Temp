#ifndef CImageProcessingEngine_h
#define CImageProcessingEngine_h

#include <stdio.h>
#include "opencv2/opencv.hpp"
using namespace cv;

#define FIND_SET		255
#define FIND_CLEAR		0

//bool FIND_PERIMETER_sort_helper(cv::Point p1, cv::Point p2);
//bool FIND_PERIMETER_isCClockwise(cv::Point p1, cv::Point p2, cv::Point p3);

/**
* CImageProcessingEngine - 
*/
class  CImageProcessingEngine 
{
public:



	CImageProcessingEngine();
	~CImageProcessingEngine();

	void FIND_REGION(cv::Mat &srcimage, cv::Mat &dstimage, int x, int y);
	void CImageProcessingEngine::FIND_PERIMETER(cv::Mat &srcimage, cv::Mat &dstimage);
	void DISPLAY_IMAGE(const cv::Mat &image, std::string const &win_name);
	void DISPLAY_PIXELS();
	void SAVE_PIXELS(const cv::Mat &image, cv::String filename);

private:
	void show_mat(const cv::Mat &image, std::string const &win_name);
	void clear_image(cv::Mat &image);

	void FIND_SetPixel(Vec3b *p);
	void FIND_ClearPixel(Vec3b *p);
	bool FIND_isPixelSet(Vec3b *p);
	

	bool FIND_REGION_isSimilar(Vec3b *p1, Vec3b *p2);
	//void FIND_REGION_helper(Vec3b p,)


}; 

#endif
