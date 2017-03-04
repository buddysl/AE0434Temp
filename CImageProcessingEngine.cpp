#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include "CImageProcessingEngine.h"
#include "opencv2/opencv.hpp"
#include <vector>

using namespace cv;


CFIND_Results::CFIND_Results() {
}

CFIND_Results::~CFIND_Results() {
}

// Every add point need to update main 'results' vector, and also 2D matrix
void CFIND_Results::addPoint(cv::Point p) {
	results.push_back(p);
	mat[p.x][p.y] = true;
}

cv::Point CFIND_Results::getPoint(int ii) {
	return results.at(ii);
}

void CFIND_Results::clear() {
	results.clear();
	for (int ii = 0; ii < mat.size(); ii++)
		for (int jj = 0; jj < mat[ii].size(); jj++)
			mat[ii][jj] = false;
}

// Note: search through the main 'results' vector is slow.  Added 2D matrix.
bool CFIND_Results::isPointInResults(cv::Point p) {
	//for (int ii = 0; ii < results.size(); ii++) {
	//	if (results.at(ii).x == p.x && results.at(ii).y == p.y)
	//		return true;
	//}
	return mat[p.x][p.y];
}

int CFIND_Results::size() {
	return results.size();
}

// resize 2D matrix and perform a clear
void CFIND_Results::resize(int nRows,int nCols) {
	mat.clear();
	mat.resize(nRows);
	for (int ii = 0; ii < nRows; ii++) mat[ii].resize(nCols);
	clear();
}

// create black and white image based pixel list
void CFIND_Results::convertToImage(cv::Mat &image) {
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
	for (int ii = 0; ii < results.size(); ii++) {
		//printf("(x,y)=(%d,%d)\n", results.at(ii).x, results.at(ii).y);
		p = image.ptr<Vec3b>(results.at(ii).x, results.at(ii).y);
		p->val[0] = 255;
		p->val[1] = 255;
		p->val[2] = 255;
	}
	return;
}


CImageProcessingEngine::CImageProcessingEngine() {

}

CImageProcessingEngine::~CImageProcessingEngine() {

}

//void CImageProcessingEngine::FIND_SetPixel(Vec3b *p) {
//	p->val[0] = FIND_SET;
//	p->val[1] = FIND_SET;
//	p->val[2] = FIND_SET;
//}
//
//void CImageProcessingEngine::FIND_ClearPixel(Vec3b *p) {
//	p->val[0] = FIND_CLEAR;
//	p->val[1] = FIND_CLEAR;
//	p->val[2] = FIND_CLEAR;
//}

//bool CImageProcessingEngine::FIND_isPixelSet(Vec3b *p) {
//	return(p->val[0] == FIND_SET && p->val[1] == FIND_SET && p->val[2] == FIND_SET);
//}

/* FIND_REGION
	Algorithm starts at (x,y) pixel, and expands to visits all 'similar' neighbours based on the function FIND_REGION_isSimilar.
	Note: future improvement is to use function pointers to allow users to provide their own functions for 'similarity'
*/
void CImageProcessingEngine::FIND_REGION(cv::Mat &srcimage, CFIND_Results &regionResults, int x, int y){

	int nRows = srcimage.rows;
	int nCols = srcimage.cols;

	// matCheck keeps track of which pixels have already been visited
	std::vector<std::vector<bool>> matCheck;
	
	// resize and initialize matCheck
	matCheck.clear();
	matCheck.resize(nRows);
	for (int ii = 0; ii < nRows; ii++) matCheck[ii].resize(nCols);

	for (int ii = 0; ii < nRows; ii++)
		for (int jj = 0; jj < nCols; jj++) {
			matCheck[ii][jj] = false;
		}

	regionResults.clear();

	// initialize stack variables
	int stackSize = 0;
	std::vector<int>stackN(nRows*nCols, -1);
	std::vector<int>stackM(nRows*nCols, -1);

	stackSize = 1;
	stackN[0] = x;
	stackM[0] = y;
	while (stackSize > 0) {
		stackSize -= 1; //pop
		int curx = stackN[stackSize];
		int cury = stackM[stackSize];
		matCheck[curx][cury] = true;

		regionResults.addPoint(Point(curx, cury));  // add current point

		//printf("x,y:%d,%d\n", curx, cury);
		Vec3b *p1 = srcimage.ptr<Vec3b>(curx, cury);
		Vec3b *p2;

		for(int ii=-1;ii<=1;ii++)	// loop through surrounding neighbours
			for (int jj = -1; jj <= 1; jj++) {
				if (ii == 0 && jj == 0) continue;	// skip if self (i.e., not neighbour)
				if (curx + ii < 0 || curx + ii >= nRows
					|| cury + jj < 0 || cury + jj >= nCols) continue;	// skip if neighbour is out of bounds
				
				p2 = srcimage.ptr<Vec3b>(curx +ii, cury+jj);
				if (FIND_REGION_isSimilar(p1, p2) && matCheck[curx +ii][cury+jj] == false) { // found new similar neighbour; add to stack 
					matCheck[curx+ii][cury+jj] = true;
					stackN[stackSize] = curx +ii;
					stackM[stackSize] = cury+jj;
					stackSize += 1;
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
void CImageProcessingEngine::FIND_PERIMETER(CFIND_Results &regionResults, CFIND_Results &perimeterResults,int width, int height) {
	
	perimeterResults.clear();

	
	Vec3b *p;
	Vec3b *p2;

	int ii, jj;
	for (int kk = 0; kk < regionResults.size(); kk++) { // loop through all pixels returned from FIND_REGION
		ii = regionResults.getPoint(kk).x;
		jj = regionResults.getPoint(kk).y;

		int count = 0;
		for (int ii2 = -1; ii2 <= 1; ii2++) { // check all surround 8 neighbours
			for (int jj2 = -1; jj2 <= 1; jj2++) {
				if (ii2 == 0 && jj2 == 0) continue;	// self, skip
				if (ii + ii2 < 0 || ii + ii2 >= width
					|| jj + jj2 < 0 || jj + jj2 >= height) continue; // skip if neighbour is out of bound
				
				if (regionResults.isPointInResults(Point(ii + ii2, jj + jj2))) count++;  // found a similar neighbour
			}
		}
		if (count < 8) { // if less than 8 similar neighbours, then is a perimeter pixel
			perimeterResults.addPoint(Point(ii, jj));
		}
	}
	return;
}


void CImageProcessingEngine::DISPLAY_IMAGE(const cv::Mat &image, std::string const &win_name) {
	namedWindow(win_name, WINDOW_AUTOSIZE);
	imshow(win_name, image);
	waitKey(0);
}

// Not working.
void CImageProcessingEngine::DISPLAY_PIXELS(CFIND_Results &results, int nRows, int nCols, std::string const &win_name) {
	Mat image(nRows,nCols, CV_8UC3, Scalar(0, 0, 0));

	results.convertToImage(image);
	namedWindow(win_name, WINDOW_AUTOSIZE);
	imshow(win_name, image);
	waitKey(0);

}

void CImageProcessingEngine::SAVE_PIXELS(const cv::Mat &image,cv::String filename) {	
	if (!image.data) {
		return;
	}
	imwrite(filename, image);
	return;
}

//// show matrix
//void CImageProcessingEngine::show_mat(const cv::Mat &image, std::string const &win_name) {
//	namedWindow(win_name, WINDOW_AUTOSIZE);
//	imshow(win_name, image);
//	//waitKey(0);
//}


//void CImageProcessingEngine::clear_image(cv::Mat &image) {
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