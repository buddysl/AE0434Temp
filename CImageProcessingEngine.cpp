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

void CFIND_Results::addPoint(cv::Point p) {
	results.push_back(p);
	mat[p.x][p.y] = true;
}

cv::Point CFIND_Results::getPoint(int ii) {
	return results.at(ii);
}
//removePoint(cv::Point);
void CFIND_Results::clear() {
	results.clear();
	for (int ii = 0; ii < mat.size(); ii++)
		for (int jj = 0; jj < mat[ii].size(); jj++)
			mat[ii][jj] = false;
}

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

void CFIND_Results::resize(int nRows,int nCols) {
	mat.clear();
	mat.resize(nRows);
	for (int ii = 0; ii < nRows; ii++) mat[ii].resize(nCols);
	clear();
}

void CFIND_Results::convertToImage(cv::Mat &image) {
	Vec3b *p;
	for (int ii = 0; ii < image.rows; ii++)
		for (int jj = 0; jj < image.cols; jj++) {
			p = image.ptr<Vec3b>(ii, jj);
			p->val[0] = 0;
			p->val[1] = 0;
			p->val[2] = 0;
		}
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

void CImageProcessingEngine::FIND_SetPixel(Vec3b *p) {
	p->val[0] = FIND_SET;
	p->val[1] = FIND_SET;
	p->val[2] = FIND_SET;
}

void CImageProcessingEngine::FIND_ClearPixel(Vec3b *p) {
	p->val[0] = FIND_CLEAR;
	p->val[1] = FIND_CLEAR;
	p->val[2] = FIND_CLEAR;
}

bool CImageProcessingEngine::FIND_isPixelSet(Vec3b *p) {
	return(p->val[0] == FIND_SET && p->val[1] == FIND_SET && p->val[2] == FIND_SET);
}

/*

*/
//void CImageProcessingEngine::FIND_REGION(cv::Mat &srcimage, cv::Mat &dstimage, int x, int y) {
//void CImageProcessingEngine::FIND_REGION(cv::Mat &srcimage, CFIND_Results &results, int x, int y);
void CImageProcessingEngine::FIND_REGION(cv::Mat &srcimage, CFIND_Results &regionResults, int x, int y){

	int nRows = srcimage.rows;
	int nCols = srcimage.cols;

	std::vector<std::vector<bool>> matCheck;
	int stackSize = 0;
	matCheck.clear();
	matCheck.resize(nRows);
	for (int ii = 0; ii < nRows; ii++) matCheck[ii].resize(nCols);

	// initialize
	Vec3b *p;
	for (int ii = 0; ii < nRows; ii++)
		for (int jj = 0; jj < nCols; jj++) {
			matCheck[ii][jj] = false;
		}

	regionResults.clear();

	std::vector<int>stackN(nRows*nCols, -1);
	std::vector<int>stackM(nRows*nCols, -1);

	Vec3b *psrc;
	Vec3b *pdst;

	stackSize = 1;
	stackN[0] = x;
	stackM[0] = y;
	while (stackSize > 0) {
		stackSize -= 1; //pop
		int curx = stackN[stackSize];
		int cury = stackM[stackSize];
		matCheck[curx][cury] = true;

		//p = dstimage.ptr<Vec3b>(curx, cury);
	
		//p->val[0] = 255;
		//p->val[1] = 255;
		//p->val[2] = 255;
		regionResults.addPoint(Point(curx, cury));

		//printf("x,y:%d,%d\n", curx, cury);
		Vec3b *p1 = srcimage.ptr<Vec3b>(curx, cury);
		Vec3b *p2;
		//int bNewNeighbour = false;

		for(int ii=-1;ii<=1;ii++)
			for (int jj = -1; jj <= 1; jj++) {
				if (ii == 0 && jj == 0) continue;
				if (curx + ii < 0 || curx + ii >= nRows
					|| cury + jj < 0 || cury + jj >= nCols) continue;
				p2 = srcimage.ptr<Vec3b>(curx +ii, cury+jj);
				if (FIND_REGION_isSimilar(p1, p2) && matCheck[curx +ii][cury+jj] == false) {
					matCheck[curx+ii][cury+jj] = true;
					stackN[stackSize] = curx +ii;
					stackM[stackSize] = cury+jj;
					stackSize += 1;
				}
			}

	}
}


//void CImageProcessingEngine::FIND_PERIMETER(cv::Mat &srcimage, cv::Mat &dstimage) {
void CImageProcessingEngine::FIND_PERIMETER(CFIND_Results &regionResults, CFIND_Results &perimeterResults,int width, int height) {
	//clear_image(dstimage);
	perimeterResults.clear();

	//int nRows = srcimage.rows;
	//int nCols = srcimage.cols;

	Vec3b *p;
	Vec3b *p2;
	//for(int ii=0;ii<srcimage.rows;ii++)
		//for (int jj = 0; jj < srcimage.cols; jj++) {
	int ii, jj;
	for (int kk = 0; kk < regionResults.size(); kk++) {
		//printf("k:%d/%d\n", kk, regionResults.size());
		//p = srcimage.ptr<Vec3b>(ii, jj);
			//if (FIND_isPixelSet(p)) {
			//if (regionResults.isPointInResults(Point(ii,jj))) {
		//printf("")
		ii = regionResults.getPoint(kk).x;
		jj = regionResults.getPoint(kk).y;
		int count = 0;
		for (int ii2 = -1; ii2 <= 1; ii2++) {
			for (int jj2 = -1; jj2 <= 1; jj2++) {
				if (ii2 == 0 && jj2 == 0) continue;
				if (ii + ii2 < 0 || ii + ii2 >= width
					|| jj + jj2 < 0 || jj + jj2 >= height) continue;
				//p2 = srcimage.ptr<Vec3b>(ii + ii2, jj + jj2);
				//if (FIND_isPixelSet(p2))count++;
				if (regionResults.isPointInResults(Point(ii + ii2, jj + jj2))) count++;
			}
		}
		if (count < 8) { // border found
			//p2 = dstimage.ptr<Vec3b>(ii, jj);
			//FIND_SetPixel(p2);
			perimeterResults.addPoint(Point(ii, jj));
		}
	}
//}
	return;
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

void CImageProcessingEngine::DISPLAY_PIXELS(CFIND_Results &results, cv::Mat &image, std::string const &win_name) {
	//Mat image(xsize,ysize, CV_8UC3, Scalar(0, 0, 0));

	results.convertToImage(image);
	namedWindow(win_name, WINDOW_AUTOSIZE);
	imshow(win_name, image);
	//waitKey(0);

}
void CImageProcessingEngine::SAVE_PIXELS(const cv::Mat &image,cv::String filename) {	
	if (!image.data) {
		return;
	}
	imwrite(filename, image);
	return;
}

// show matrix
void CImageProcessingEngine::show_mat(const cv::Mat &image, std::string const &win_name) {
	namedWindow(win_name, WINDOW_AUTOSIZE);
	imshow(win_name, image);
	//waitKey(0);
}


void CImageProcessingEngine::clear_image(cv::Mat &image) {
	Vec3b *p;
	for (int ii = 0; ii < image.rows; ii++)
		for (int jj = 0; jj < image.cols; jj++) {
			p = image.ptr<Vec3b>(ii, jj);
			p->val[0] = 0;
			p->val[1] = 0;
			p->val[2] = 0;
		}
	return;
}

bool CImageProcessingEngine::FIND_REGION_isSimilar(Vec3b *p1, Vec3b *p2) {
	if (p1->val[0] == p2->val[0] && p1->val[1] == p2->val[1] && p1->val[2] == p2->val[2]) {
		return true;
	}
	else {
		return false;
	}
}