#ifndef CImageProcessingEngine_h
#define CImageProcessingEngine_h

#include <stdio.h>
#include "opencv2/opencv.hpp"
#include <vector>

using namespace cv;

/********************************************************************************
* CFIND_Results class.  
* 
* This class stores and manipulates the (x,y) pixels of an image from the FIND_ operations.
********************************************************************************/
class CFIND_Results {
public:

	/** Constructor*/
	CFIND_Results();

	/** Destructor*/
	~CFIND_Results();

	/** Add a (x,y) point */
	void addPoint(Point p);

	/** Add a vector of (x,y) points */
	void addVectorOfPoints(std::vector<Point> pts);

	/** Remove a (x,y) point */
	void removePoint(Point p);

	/** Get the ii-th point.  Use numPoints() to determine number of points in the results */
	Point getPoint(int ii);

	/** Clear the results storage */
	void clear();
	
	/** Determine if a point is in the results storage.  
		Returns -1 if not found.
		Returns -2 if point is out of bounds
		Otherwise, returns the index of the point.
	*/
	int isPointInResults(Point p);

	/** Returns number of points in the results*/
	int numPoints();

	/** Returns the image size that this results object needs to capture
	*/
	Point size();

	/** Set the size of the image that this results image needs to capture.
		Note: everytime 'resize' is called, all previous data is cleared.  
	@param nRows - number of rows
	@param nCols - number of columns
	*/
	void resize(int nRows,int nCols);
	
	/** Converts the results from the FIND_ operations to a black and white image.  
		All pixels stored in the results object are coloured while, otherwise black.
	@param image - black and white image
	*/
	void convertToImage(Mat &image);

	/** Load previous results from file, which is a black and white image.
	@param image - black and white image
	*/
	bool loadFromImage(Mat &image);

	/** Called to organize the data such that they draw a continuous path.
		Obsolete
	*/
	//void sortAlongPath();

	/** Copy the connect from current results object to another 'dst' object
	@ param dst - destination object to copy info to.
	*/
	void copyTo(CFIND_Results &dst);
	
private:
	/** array storing the pixels*/
	std::vector<Point> results;

	/** 2D matrix to optimize the 'isPointInResults' search function*/
	std::vector<std::vector<int>> mat;

	/** for development convenience	*/
	
	/** general function to convert any array to a black and white image
	@ param v : vector
	@ param image : destination image
	*/
	void convertVectorToImage(std::vector<Point> v, Mat &image);

	/* for development convenience */
	//void display_pixels(std::vector<Point> v, std::string const &win_name, String const &filename);
};


/********************************************************************************
* CImageProcessingEngine
*
* Main engine to implement the functions
********************************************************************************/
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
	void FIND_REGION(Mat &srcimage, CFIND_Results &regionResults, int x, int y);

	/** FIND_PERIMETER
	@param regionResults - input pixels returned by FIND_REGION
	@param perimeterResults - output pixels returned by FIND_PERIMETER	
	*/
	void FIND_PERIMETER(CFIND_Results &regionResults, CFIND_Results &perimeterResults);
	

	/** FIND_SMOOTH_PERIMETER
	@param regionResults - input pixels returned by FIND_REGION
	@param smoothPerimeterResults - output pixels returned by FIND_SMOOTH_PERIMETER
	*/
	void FIND_SMOOTH_PERIMETER(CFIND_Results &regionResults, CFIND_Results &smoothPerimeterResults);

	/** DISPLAY_IMAGE
	@param image - source image
	@param win_name - window name of display
	*/
	void DISPLAY_IMAGE(const Mat &image, std::string const &win_name);

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
	void SAVE_PIXELS(CFIND_Results &results, String filename);

private:

	//void show_mat(const Mat &image, std::string const &win_name);
	//void clear_image(Mat &image);

	
	//***************************************
	// FIND_REGION helper functions

	// function used to determine if two pixels are similar or not.
	bool FIND_REGION_isSimilar(Vec3b *p1, Vec3b *p2);


	//***************************************
	// FIND_PERIMETER helper functions



	//***************************************
	// FIND_SMOOTH_PERIMETER helper functions
	
	/** FIND_SMOOTH_PERIMETER_cleanSmallArtifacts - removes all pixels with less than 'threshold' number of neighbors.  
	@param regionResults - input pixels from FIND_REGION.  Note: this function directly modifies the object.
	@param threshold - threshold for number of neighbors
	*/
	void FIND_SMOOTH_PERIMETER_cleanSmallArtifacts(CFIND_Results &regionResults, int threshold);

	/** FIND_SMOOTH_PERIMETER_findConvexHull - finds the convex hull of the pixels
	@param results
	*/
	std::vector<Point> FIND_SMOOTH_PERIMETER_findConvexHull(CFIND_Results &results);
	
	/** FIND_SMOOTH_PERIMETER_evaluateCardinal2D_oneSegment - performs curve fitting using splines between two points P1 and P2.
	@param P0,P1,P2,P3  are given four (x,y) points. Note: P0 and P3 are used to calculate the slope of the endpoints
	@param T is tension (T=0 for Catmull-Rom type)
	@param u is parameter at which spline is evaluated (used to draw spline)
	*/
	Point FIND_SMOOTH_PERIMETER_evaluateCardinal2D_oneSegment(Point p0, Point p1, Point p2, Point p3, double T, double u);
	
	/** FIND_SMOOTH_PERIMETER_evaluateCardinal2D_all - performs curve fitting using splines on a set of points.
	@param vertices - set of points
	@param T is tension (T=0 for Catmull-Rom type)
	@param n is the number of (sub)points to draw for the splines between two points
	*/
	std::vector<Point> FIND_SMOOTH_PERIMETER_evaluateCardinal2D_all(std::vector<Point> vertices, double T,  int n);


}; 

#endif
