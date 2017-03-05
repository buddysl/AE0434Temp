#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include "CImageProcessingEngine.h"
#include "opencv2/opencv.hpp"
#include <vector>

using namespace cv;


/********************************************************************************
*
*  CFIND_Results Class
*
********************************************************************************/
CFIND_Results::CFIND_Results() {
}

CFIND_Results::~CFIND_Results() {
}

// Function to add a point to this object.
// Note: function needs to make update in two places:
//		- results vector
//		- mat matrix
void CFIND_Results::addPoint(Point p) {
	int flag = isPointInResults(p);
	if (flag < 0 && flag !=-2) {
		results.push_back(p);
		mat[p.x][p.y] = results.size() - 1;
	}
}

void CFIND_Results::addVectorOfPoints(std::vector<Point> pts) {
	for (int ii = 0; ii < pts.size(); ii++) {		
		addPoint(pts[ii]);
	}
}

// Remove point - achieved by swapping the element to be removed with the last element, and call pop_back().
void CFIND_Results::removePoint(Point p) {
	int ii = isPointInResults(p);
	if (ii < 0) return;

	if (ii == results.size() - 1) {
		mat[p.x][p.y] = -1; 
		results.pop_back();		
	}
	else {
		mat[p.x][p.y] = -1;
		results[ii] = results.back();
		mat[results[ii].x][results[ii].y] = ii;
		results.pop_back();
	}
}

Point CFIND_Results::getPoint(int ii) {
	return results.at(ii);
}

void CFIND_Results::clear() {
	results.clear();
	int nRows = mat.size();
	if (nRows <= 0) return;

	int nCols = mat[0].size();
	mat.clear();
	// slow!!
	//for (int ii = 0; ii < mat.size(); ii++)
	//	for (int jj = 0; jj < mat[ii].size(); jj++)
	//		mat[ii][jj] = false;
	//
	// faster
	mat.resize(nRows, std::vector<int>(nCols, -1));

}

// Function to search if a pixel is stored.
// Note that for large images with lots of similar pixels, the search through the main 'results' vector is slow.  
// So this class also uses a redundant 2D matrix that stores the same info as 'results' vector, but search is faster.
int CFIND_Results::isPointInResults(Point p) {
	// slow!
	//for (int ii = 0; ii < results.size(); ii++) {
	//	if (results.at(ii).x == p.x && results.at(ii).y == p.y)
	//		return true;
	//}
	// faster!
	if (p.x < 0 || p.x >= mat.size() || p.y < 0 || p.y >= mat[0].size()) return -2;
	return mat[p.x][p.y];
}

int CFIND_Results::numPoints() {
	return results.size();
}

Point CFIND_Results::size() {
	if (mat.size() == 0) return Point(0, 0);
	return Point(mat.size(), mat[0].size());
}
// resize 2D matrix and perform a clear
void CFIND_Results::resize(int nRows,int nCols) {
	mat.resize(nRows, std::vector<int>(nCols, -1));		// default to -1; i.e., point not stored
	results.clear();
}

// Create a black-and-white image.  Pixels stored in the object are white.  Pixels not sored are black.
void CFIND_Results::convertVectorToImage(std::vector<Point> v, Mat &image) {
	Vec3b *p;
	// clear image: make all pixels black
	for (int ii = 0; ii < image.rows; ii++)
		for (int jj = 0; jj < image.cols; jj++) {
			p = image.ptr<Vec3b>(ii, jj);
			p->val[0] = 0;
			p->val[1] = 0;
			p->val[2] = 0;
		}

	// make all pixels in list white
	for (int ii = 0; ii < v.size(); ii++) {
		//printf("(x,y)=(%d,%d)\n", results.at(ii).x, results.at(ii).y);
		p = image.ptr<Vec3b>(v.at(ii).x, v.at(ii).y);
		p->val[0] = 255;
		p->val[1] = 255;
		p->val[2] = 255;
	}
	return;
}

// create black and white image based on list of pixels stored in this object
void CFIND_Results::convertToImage(Mat &image) {
	convertVectorToImage(results, image);
	return;
}


bool CFIND_Results::loadFromImage(Mat &image) {
	resize(image.rows, image.cols);
	Vec3b *p;
	// clear image: make all pixels black
	for (int ii = 0; ii < image.rows; ii++)
		for (int jj = 0; jj < image.cols; jj++) {
			p = image.ptr<Vec3b>(ii, jj);
			if (p->val[0] == 255 && p->val[1] == 255 && p->val[2] == 255) {
				addPoint(Point(ii, jj));
			}
			else if (p->val[0] == 0 && p->val[1] == 0 && p->val[2] == 0) {
			}
			else {
				// not a CFIND_Results image
				return false;
			}
		}
}

/* for development convenience */
//void CFIND_Results::display_pixels(std::vector<Point> v, std::string const &win_name, String const &filename) {
//
//	Mat image(size().x, size().y, CV_8UC3, Scalar(0, 0, 0));
//
//	convertVectorToImage(v,image);
//	namedWindow(win_name, WINDOW_AUTOSIZE);
//	imshow(win_name, image);
//	waitKey(0);
//
//	imwrite(filename, image);
//
//}

void CFIND_Results::copyTo(CFIND_Results &dst) {
	dst.resize(mat.size(), mat[0].size());
	for (int ii = 0; ii < results.size(); ii++)
		dst.addPoint(results[ii]);
}

/** Obsolete */
//void CFIND_Results::sortAlongPath() {
//	
//	int ii_origin = 0;
//	for (int ii = 1 ; ii < results.size(); ii++) {
//		if (results[ii].x < results[ii_origin].x)
//			ii_origin = ii;
//		else if (results[ii].x == results[ii_origin].x && results[ii].y < results[ii_origin].y)
//			ii_origin = ii;
//	}
//
//	std::vector<bool> check(results.size(), false);
//
//	std::vector<Point>newResults;
//	newResults.clear();
//	//results.swap()
//	newResults.push_back(results[ii_origin]);
//
//	//check[ii_origin] = true;
//
//	int count = results.size()-1;
//	int count2 = 0;
//	while (count > 0) {
//		count--;
//
//		std::vector<Point> idx(8);
//		idx[0] = Point(0, -1);
//		idx[1] = Point(1, 0);
//		idx[2] = Point(0, +1);
//		idx[3] = Point(-1, 0);
//		idx[4] = Point(-1, -1);
//		idx[5] = Point(1, -1);
//		idx[6] = Point(1, +1);
//		idx[7] = Point(-1, +1);
//
//		Point curp = newResults.back();
//		int ii, jj;
//		for(int kk=0;kk<8;kk++){
//			ii = idx[kk].x;
//			jj = idx[kk].y;
//				if (ii == 0 && jj == 0) continue;
//				Point neighbourp = Point(curp.x + ii, curp.y + jj);
//				//printf("#%d:(%d,%d),(%d,%d)\n", count,curp.x, curp.y, curp.x + ii, curp.y + jj);
//				if (isPointInResults(neighbourp)>=0) {
//					if (check[isPointInResults(neighbourp)] == false) {
//						newResults.push_back(neighbourp);
//						check[isPointInResults(neighbourp)] = true;
//						break;
//						if (neighbourp.x == newResults[0].x && neighbourp.y == newResults[0].y) {
//							printf("break\n");
//							break;
//						}
//					}
//				}
//		}
//	}
//	printf("size of results, newresults: %d,%d", results.size(), newResults.size());
//	//display_pixels(results, "CFIND_results: results","temp1.png");
//	//display_pixels(newResults, "CFIND_results: new results","temp2.png");
//	results = newResults;
//}


/********************************************************************************
* 
*  CImageProcessingEngine Class
*
********************************************************************************/
CImageProcessingEngine::CImageProcessingEngine() {

}

CImageProcessingEngine::~CImageProcessingEngine() {

}


/* FIND_REGION
	Algorithm starts at (x,y) pixel, and expands to visits all 'similar' neighbours based on the function FIND_REGION_isSimilar.
	Note: future improvement is to use function pointers to allow users to provide their own functions for 'similarity'
*/
void CImageProcessingEngine::FIND_REGION(Mat &srcimage, CFIND_Results &regionResults, int x, int y) {

	int nRows = srcimage.rows;
	int nCols = srcimage.cols;

	regionResults.resize(nRows,nCols);

	// initialize stack variables
	std::vector<Point>stack;
	stack.clear();

	Point p;

	stack.push_back(Point(x, y));
	while (!stack.empty()) {
		p = stack.back();
		stack.pop_back();
		int curx = p.x;
		int cury = p.y;
		regionResults.addPoint(p);  // add current point

		//printf("x,y:%d,%d\n", curx, cury);
		Vec3b *p1 = srcimage.ptr<Vec3b>(curx, cury);
		Vec3b *p2;

		for (int ii = -1; ii <= 1; ii++) {	// loop through surrounding neighbours
			for (int jj = -1; jj <= 1; jj++) {
				if (ii == 0 && jj == 0) continue;	// skip if self (i.e., not neighbour)
				if (curx + ii < 0 || curx + ii >= nRows
					|| cury + jj < 0 || cury + jj >= nCols) continue;	// skip if neighbour is out of bounds

				p2 = srcimage.ptr<Vec3b>(curx + ii, cury + jj);
				if (FIND_REGION_isSimilar(p1, p2) && regionResults.isPointInResults(Point(curx + ii, cury + jj)) < 0) {
					stack.push_back(Point(curx + ii, cury + jj));
				}
			}
		}
	}
}


/* FIND_PERIMETER
Algorithm goes through all pixels returned from FIND_REGION.  For each pixel, check all surrounding neighbours, and flag the pixel as a
perimeter pixel if either
i) the pixel has a neighbour that is not similar (i.e., pixel not in the results from FIND_REGION); and/or
ii) the pixel on the edge of the image (i.e., has a neighbour that is out of bounds).
Note: based on above conditions, a perimeter pixel is one that has less than 8 similar neighbours.
*/
void CImageProcessingEngine::FIND_PERIMETER(CFIND_Results &regionResults, CFIND_Results &perimeterResults) {

	perimeterResults.resize(regionResults.size().x, regionResults.size().y);
	int width = regionResults.size().x;
	int height = regionResults.size().y;

	int ii, jj;
	for (int kk = 0; kk < regionResults.numPoints(); kk++) { // loop through all pixels returned from FIND_REGION
		ii = regionResults.getPoint(kk).x;
		jj = regionResults.getPoint(kk).y;

		int count = 0;
		for (int ii2 = -1; ii2 <= 1; ii2++) { // check all surround 8 neighbours
			for (int jj2 = -1; jj2 <= 1; jj2++) {
				if (ii2 == 0 && jj2 == 0) continue;	// self, skip
				if (ii + ii2 < 0 || ii + ii2 >= width
					|| jj + jj2 < 0 || jj + jj2 >= height) continue; // skip if neighbour is out of bound

				if (regionResults.isPointInResults(Point(ii + ii2, jj + jj2)) >= 0) count++;  // found a similar neighbour
			}
		}
		if (count < 8) { // if less than 8 similar neighbours, then is a perimeter pixel
			perimeterResults.addPoint(Point(ii, jj));
		}
	}
	return;
}

// FIND_SMOOTH_PERIMETER
// 

// used by FIND_SMOOTH_PERIMETER_findConvexHull to sort points
bool FIND_SMOOTH_PERIMETER_sort_helper(Point p1, Point p2) {
	if (p1.x == p2.x)
		return p1.y > p2.y;
	else
		return p1.x < p2.x;
}

// used by FIND_SMOOTH_PERIMETER_findConvexHull to determine if points are moving in a counter clockwise manner.
// Angle between two vectors can be computed by cross product. AxB=|A||B|sin(ang).
// For our purposes, we only need to know if the angle is >= 0deg or < 0deg, so AxB >= 0 or <0.
bool FIND_SMOOTH_PERIMETER_isCClockwise(Point p1, Point p2, Point p3) {
	return (p2.x - p1.x) * (p3.y - p1.y) >= (p2.y - p1.y) * (p3.x - p1.x);
}

/* FIND_SMOOTH_PERIMETER_cleanSmallArtifacts
Algorithm goes through all pixels returned from FIND_REGION.  For each pixel, algorithm checks all surrounding neighbours, and flag the pixel as a
'small artifact' pixel if the number of neighbors is less than give threshold
*/
void CImageProcessingEngine::FIND_SMOOTH_PERIMETER_cleanSmallArtifacts(CFIND_Results &regionResults, int threshold) {
	Point p;
	bool changedResults = true;
	while (changedResults == true) {  // every time a small artifact is found and remove, will need to scan through the whole image again
		changedResults = false;

		for (int ii = 0; ii < regionResults.numPoints(); ii++) {
			p = regionResults.getPoint(ii);

			// count number of neighbours
			int count = 0;
			for (int ii2 = -1; ii2 <= 1; ii2++) { // check all surround 8 neighbours
				for (int jj2 = -1; jj2 <= 1; jj2++) {
					if (ii2 == 0 && jj2 == 0) continue;	// self, skip
					if (p.x + ii2 < 0 || p.x + ii2 >= regionResults.size().x
						|| p.y + jj2 < 0 || p.y + jj2 >= regionResults.size().y) continue; // skip if neighbour is out of bound

					if (regionResults.isPointInResults(Point(p.x + ii2, p.y + jj2)) >= 0) count++;  // found a similar neighbour
				}
			}
			if (count < threshold) { // less than 'threshold' number of neighbours, small artifact pixel found
				regionResults.removePoint(p);
				changedResults = true;
			}
		}
	}
}

/* FIND_SMOOTH_PERIMETER_findConvexHull
The algorithm returns the vertices of the convex hull in a clockwise order. 
To do so, the algorithm first sorts all pixels in the x-direction, and then the y-direction.  The algorithm then first builds the upper hull by
traversing each pixel and if a counter clockwise direction is made, the algorithm backtracks and one-by-one removes the previous pixels until the counter clockwise
direction is fixed.  The algorithm then repeats for the lower hall.
*/
std::vector<Point> CImageProcessingEngine::FIND_SMOOTH_PERIMETER_findConvexHull(CFIND_Results &results) {
	std::vector<Point> resultpts;
	resultpts.clear();
	for (int ii = 0; ii < results.numPoints(); ii++)
		resultpts.push_back(results.getPoint(ii));

	sort(resultpts.begin(), resultpts.end(), FIND_SMOOTH_PERIMETER_sort_helper); // sort

	std::vector<Point> hullpts;
	hullpts.clear();
	hullpts.push_back(resultpts[0]);
	hullpts.push_back(resultpts[1]);
	// build upper hull
	for (int ii = 2; ii < resultpts.size(); ii++) {
		if (hullpts.size() > 2) {
			while (hullpts.size()>=2 && FIND_SMOOTH_PERIMETER_isCClockwise(hullpts[hullpts.size() - 2], hullpts[hullpts.size() - 1], resultpts[ii])) {
				hullpts.pop_back();
			}
			hullpts.push_back(resultpts[ii]);			
		}
		else
			hullpts.push_back(resultpts[ii]);
	}

	int numUpperHullPt = hullpts.size();
	// build lower hull
	for (int ii = resultpts.size() - 1; ii >= 0; ii--) {
		while (hullpts.size() >= numUpperHullPt + 1 && FIND_SMOOTH_PERIMETER_isCClockwise(hullpts[hullpts.size() - 2], hullpts[hullpts.size() - 1], resultpts[ii]))
			hullpts.pop_back();
		hullpts.push_back(resultpts[ii]);
	}

	//if (hullpts.back().x == hullpts[0].x && hullpts.back().y == hullpts[0].y) hullSize -= 1;
	
	return hullpts;
}

/* FIND_SMOOTH_PERIMETER_evaluateCardinal2D_oneSegment
	Algorithm extracted from 
	https://www.mathworks.com/matlabcentral/fileexchange/7078-cardinal-spline--catmull-rom--spline
*/
Point CImageProcessingEngine::FIND_SMOOTH_PERIMETER_evaluateCardinal2D_oneSegment(Point p0, Point p1, Point p2, Point p3, double T, double u) {
	double s = (1 - T) / 2;
	Matx44d MC;
	MC(0,0) = -s;	MC(0,1) = 2-s;	MC(0,2) = s-2;		MC(0,3) = s;
	MC(1,0) = 2*s;	MC(1,1) = s-3;	MC(1,2) = 3-2*s;	MC(1,3) = -s;
	MC(2,0) = -s;	MC(2,1) = 0;	MC(2,2) = s;		MC(2,3) = 0;
	MC(3,0) = 0;	MC(3,1) = 1;	MC(3,2) = 0;		MC(3,3) = 0;

	Vec4d GHx, GHy;
	GHx[0] = p0.x;	GHx[1] = p1.x;	GHx[2] = p2.x;	GHx[3] = p3.x;
	
	GHy[0] = p0.y;	GHy[1] = p1.y;	GHy[2] = p2.y;	GHy[3] = p3.y;

	Vec4d U;
	U[0] = u*u*u;	U[1] = u*u;		U[2] = u;		U[3] = 1;

	double xt, yt;
	
	Vec4d MCGHx, MCGHy;

	for (int ii = 0; ii < 4; ii++) {
		MCGHx[ii] = 0;
		MCGHy[ii] = 0;
		for (int jj = 0; jj < 4; jj++) {
			MCGHx[ii] += MC(ii,jj) * GHx[jj];
			MCGHy[ii] += MC(ii,jj) * GHy[jj];
		}
	}

	xt = 0;
	yt = 0;
	for (int ii = 0; ii < 4; ii++) {
		xt += U[ii] * MCGHx[ii];
		yt += U[ii] * MCGHy[ii];
	}
	return Point(xt, yt);
}

/* FIND_SMOOTH_PERIMETER_evaluateCardinal2D_all
Algorithm adapted from
https://www.mathworks.com/matlabcentral/fileexchange/7078-cardinal-spline--catmull-rom--spline
*/
std::vector<Point> CImageProcessingEngine::FIND_SMOOTH_PERIMETER_evaluateCardinal2D_all(std::vector<Point> vertices, double T, int n) {
	std::vector<Point> spline;
	Point p0, p1, p2, p3;
	Point pspline;

	std::vector<Point> vertices2;
	vertices2.push_back(vertices[0]);
	for(int ii=0;ii<vertices.size();ii++) vertices2.push_back(vertices[ii]);
	vertices2.push_back(vertices[0]);
	vertices2.push_back(vertices[0]);
	
	spline.clear();
	for (int ii = 0; ii < vertices2.size() - 3; ii++) {
		p0 = vertices2[ii];
		p1 = vertices2[ii+1];
		p2 = vertices2[ii+2];
		p3 = vertices2[ii+3];

		pspline = FIND_SMOOTH_PERIMETER_evaluateCardinal2D_oneSegment(p0, p1, p2, p3,T,0);
		spline.push_back(pspline);
		for (int jj = 0; jj < n; jj++) {
			double u = (double)jj / ((double)n - 1.0);
			pspline = FIND_SMOOTH_PERIMETER_evaluateCardinal2D_oneSegment(p0, p1, p2, p3,T,u);
			spline.push_back(pspline);
		}
	}

	return spline;
}

/* FIND_SMOOTH_PERIMETER
	Algorithm performs the following functions in order to provide  smooth perimeter
	- remove small artifacts
	- find the resulting perimeter
	- find convex hull of the perimeter
	- curve fit the convex hull
	- store the results in smoothPerimeterResults
*/
void CImageProcessingEngine::FIND_SMOOTH_PERIMETER(CFIND_Results &regionResults, CFIND_Results &smoothPerimeterResults) {
	smoothPerimeterResults.resize(regionResults.size().x, regionResults.size().y);
	
	CFIND_Results tempResults;
	regionResults.copyTo(tempResults);

	FIND_SMOOTH_PERIMETER_cleanSmallArtifacts(tempResults,3);
	//DISPLAY_PIXELS(tempResults, "Internal:smooth");

	CFIND_Results perimeterResults;
	FIND_PERIMETER(tempResults, perimeterResults); 

	/*CFIND_Results tempResults2, tempResults3;
	tempResults2.resize(smoothPerimeterResults.size().x, smoothPerimeterResults.size().y);
	tempResults3.resize(smoothPerimeterResults.size().x, smoothPerimeterResults.size().y);
*/
	std::vector<Point> convexHull = FIND_SMOOTH_PERIMETER_findConvexHull(perimeterResults);
	//tempResults2.addVectorOfPoints(convexHull);
	//DISPLAY_PIXELS(tempResults2, "Internal:smooth 2");
	
	std::vector<Point> spline = FIND_SMOOTH_PERIMETER_evaluateCardinal2D_all(convexHull, 0, 100);
	//tempResults3.addVectorOfPoints(spline);
	//DISPLAY_PIXELS(tempResults3, "Internal:smooth 3");

	
	// store the results
	smoothPerimeterResults.clear();
	smoothPerimeterResults.addVectorOfPoints(spline);
	
	//DISPLAY_PIXELS(perimeterResults, "Internal:smooth1");
	//DISPLAY_PIXELS(smoothPerimeterResults, "Internal:smooth0");
}



void CImageProcessingEngine::DISPLAY_IMAGE(const Mat &image, std::string const &win_name) {
	namedWindow(win_name, WINDOW_AUTOSIZE);
	imshow(win_name, image);
	waitKey(0);
}

// Not working (fixed)
void CImageProcessingEngine::DISPLAY_PIXELS(CFIND_Results &results, std::string const &win_name) {
	Mat image(results.size().x,results.size().y, CV_8UC3, Scalar(0, 0, 0));

	results.convertToImage(image);
	namedWindow(win_name, WINDOW_AUTOSIZE);
	imshow(win_name, image);
	waitKey(0);

}

void CImageProcessingEngine::SAVE_PIXELS(CFIND_Results &results, String filename) {
	Mat image(results.size().x, results.size().y, CV_8UC3, Scalar(0, 0, 0));

	results.convertToImage(image);
	
	if (!image.data) {
		return;
	}
	imwrite(filename, image);
	return;
}

//// show matrix
//void CImageProcessingEngine::show_mat(const Mat &image, std::string const &win_name) {
//	namedWindow(win_name, WINDOW_AUTOSIZE);
//	imshow(win_name, image);
//	//waitKey(0);
//}


//void CImageProcessingEngine::clear_image(Mat &image) {
//	Vec3b *p;
//	for (int ii = 0; ii < image.rows; ii++)
//		for (int jj = 0; jj < image.cols; jj++) {
//			p = image.ptr<Vec3b>(ii, jj);
//			p->val[0] = 0;
//			p->val[1] = 0;
//			p->val[2] = 0;
//		}
//	return;
//}

// Sample 'similarity' function.
bool CImageProcessingEngine::FIND_REGION_isSimilar(Vec3b *p1, Vec3b *p2) {
	if (p1->val[0] == p2->val[0] && p1->val[1] == p2->val[1] && p1->val[2] == p2->val[2]) {
		return true;
	}
	else {
		return false;
	}
}