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

	void addVectorOfPoints(std::vector<cv::Point> pts);

	void removePoint(cv::Point p);

	/** Get the ii-th point from the results.  Use int size() to determine number of points in the results */
	cv::Point getPoint(int ii);

	/** Clear the results */
	void clear();
	
	/** Determine if a point is in the results.  Returns -1 if not found, otherwise returns an index.*/
	int isPointInResults(cv::Point p);

	/** Returns number of points in the results*/
	int numPoints();

	cv::Point size();

	/** Resize 2D matrix introduced to optimize speed of 'isPointInResults'.
		Note: everytime 'resize' is called, all the data is cleared.  So do not resize in the middle of a FIND_ operation.
		Future improvement: dynamically resize 2D matrix based on 'addPoint'

	@param nRows - number of rows
	@param nCols - number of columns
	*/
	void resize(int nRows,int nCols);
	
	/** Converts the results from the FIND_ operations to a black and white image.  
		All pixels returned by the FIND_ operations are coloured while, otherwise black.
	@param image - black and white image
	*/
	void convertToImage(cv::Mat &image);


	bool loadFromImage(cv::Mat &image);

	void sortAlongPath();

	void copyTo(CFIND_Results &dst);
	
private:
	/** array storing the pixels returned by FIND_*/
	std::vector<cv::Point> results;

	/** 2D matrix to optimize the 'isPointInResults' */
	std::vector<std::vector<int>> mat;

	void convertVectorToImage(std::vector<cv::Point> v, cv::Mat &image);
	void display_pixels(std::vector<cv::Point> v, std::string const &win_name, cv::String const &filename);
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
	void FIND_PERIMETER(CFIND_Results &regionResults, CFIND_Results &perimeterResults);
	

	//void FIND_SMOOTH_PERIMETER(CFIND_Results &perimeterResults, CFIND_Results &smoothPerimeterResults);
	void FIND_SMOOTH_PERIMETER(CFIND_Results &regionResults, CFIND_Results &smoothPerimeterResults);

	/** DISPLAY_IMAGE
	@param image - source image
	@param win_name - window name of display
	*/
	void DISPLAY_IMAGE(const cv::Mat &image, std::string const &win_name);

	/** DISPLAY_PIXELS
	@param results - results from FIND_operation
	@param image - image created from results
	@param win_name - window name of display
	*/
	void DISPLAY_PIXELS(CFIND_Results &results, std::string const &win_name);

	/** SAVE_PIXELS
	@param image - source image
	@param filename - filename
	*/
	void SAVE_PIXELS(CFIND_Results &results, cv::String filename);

private:

	//void show_mat(const cv::Mat &image, std::string const &win_name);
	//void clear_image(cv::Mat &image);

	//void FIND_SetPixel(Vec3b *p);
	//void FIND_ClearPixel(Vec3b *p);
	//bool FIND_isPixelSet(Vec3b *p);
	
	bool FIND_REGION_isSimilar(Vec3b *p1, Vec3b *p2);

	double FIND_SMOOTH_PERIMETER_isAngleSmallerThan(cv::Point2d a, cv::Point2d b, double angle_threshold);
	//double FIND_SMOOTH_PERIMETER_dataFittingCost(CFIND_Results &results, int i1, int i2);
	double FIND_SMOOTH_PERIMETER_dataFittingCost(std::vector<cv::Point2d>data, cv::Point2d c0, cv::Point2d c1, cv::Point2d c2, cv::Point2d c3);
	double FIND_SMOOTH_PERIMETER_distanceToBezierCurve(cv::Point2d pi, cv::Point2d c0, cv::Point2d c1, cv::Point2d c2, cv::Point2d c3 );
	double FIND_SMOOTH_PERIMETER_distance(cv::Point2d p1, cv::Point2d p2);
	cv::Vec4d FIND_SMOOTH_PERIMETER_cubicBezierCurveFitting1D(std::vector<double> data);
	std::vector<cv::Point2d> FIND_SMOOTH_PERIMETER_cubicBezierCurveFitting2D(std::vector<cv::Point2d> data);

	void FIND_SMOOTH_PERIMETER_cleanSmallArtifacts(CFIND_Results &regionResults, int threshold);
	std::vector<cv::Point> FIND_SMOOTH_PERIMETER_findConvexHull(CFIND_Results &results);
	cv::Point FIND_SMOOTH_PERIMETER_evaluateCardinal2D_oneSegment(Point p0, Point p1, Point p2, Point p3, double T, double u);
	std::vector<cv::Point> FIND_SMOOTH_PERIMETER_evaluateCardinal2D_all(std::vector<cv::Point> vertices, double T,  int n);

}; 

#endif
