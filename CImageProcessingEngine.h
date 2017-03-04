#ifndef CImageProcessingEngine_h
#define CImageProcessingEngine_h

#include <stdio.h>
#include "opencv2/opencv.hpp"
#include <vector>
using namespace cv;

#define FIND_SET		255
#define FIND_CLEAR		0

/**
* CFIND_Results class.  This class stores the (x,y) pixels of an image from the FIND_ operations.
*/
class CFIND_Results {
public:

	/** Constructor*/
	CFIND_Results();

	/** Destructor*/
	~CFIND_Results();

	/** Add an (x,y) point in the image satifying the FIND_ operation to the results
	@param p (x,y) point in the image.
	*/
	void addPoint(cv::Point p);

	/** Get the ii-th point from the results.  Use int size() to determine number of points in the results */
	cv::Point getPoint(int ii);
	
	/** Clear the results */
	void clear();
	
	/** Determine if a point is in the results*/
	bool isPointInResults(cv::Point p);

	/** Returns number of points in the results*/
	int size();

	void resize(int nRows,int nCols);
	
	/** Converts the results from the FIND_ operations to a black and white image.  
		All pixels returned by the FIND_ operations are coloured while, otherwise black.
	@param image - black and white image
	*/
	void convertToImage(cv::Mat &image);
	
private:
	/** array storing the pixels returned by FIND_*/
	std::vector<cv::Point> results;

	/** 2D matrix to optimize the 'isPointInResults' */
	std::vector<std::vector<bool>> mat;
};


/**
* CImageProcessingEngine - main engine to implement the functions
*/
class  CImageProcessingEngine 
{
public:
	/** Constructor*/
	CImageProcessingEngine();
	
	/** Destructor*/
	~CImageProcessingEngine();

	/** FIND_REGION
	@param srcimage - source image
	@param regionResults - stores the pixels returned by the function
	@param x - x coordinate of main pixel
	@param y - y coordinate of main pixel
	*/
	void FIND_REGION(cv::Mat &srcimage, CFIND_Results &regionResults, int x, int y);

	/** FIND_PERIMETER
	@param srcimage - source image
	@param regionResults - stores the pixels returned by the function
	@param x - x coordinate of main pixel
	@param y - y coordinate of main pixel
	*/
	void FIND_PERIMETER(CFIND_Results &regionResults, CFIND_Results &perimeterResults, int width,int height);
	void DISPLAY_IMAGE(const cv::Mat &image, std::string const &win_name);
	void DISPLAY_PIXELS(CFIND_Results &results, cv::Mat &image, std::string const &win_name);
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
