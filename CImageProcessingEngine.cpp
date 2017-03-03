#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include "CImageProcessingEngine.h"
#include "opencv2/opencv.hpp"
#include <vector>

using namespace cv;

CImageProcessingEngine::CImageProcessingEngine() {

}

CImageProcessingEngine::~CImageProcessingEngine() {

}

void CImageProcessingEngine::FIND_REGION(cv::Mat &srcimage, cv::Mat &dstimage, int x, int y) {

	int channels = srcimage.channels();
	int nRows = srcimage.rows;
	int nCols = srcimage.cols;
	int ii, jj;


	std::vector<std::vector<bool>> matCheck;
	int stackSize = 0;
	matCheck.clear();
	matCheck.resize(nRows);
	for (ii = 0; ii < nRows; ii++) matCheck[ii].resize(nCols);

	// initialize
	Vec3b *p;
	for (ii = 0; ii < nRows; ii++)
		for (jj = 0; jj < nCols; jj++) {
			matCheck[ii][jj] = false;
			p = dstimage.ptr<Vec3b>(ii, jj);
			p->val[0] = 0;
			p->val[1] = 0;
			p->val[2] = 0;
		}

	std::vector<int>stackN(nRows*nCols, -1);
	std::vector<int>stackM(nRows*nCols, -1);

	switch (channels) {
	case 1: //            gray scale image
		break;
	case 3:  //            RGB image
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

			p = dstimage.ptr<Vec3b>(curx, cury);
	
			p->val[0] = 255;
			p->val[1] = 255;
			p->val[2] = 255;

			//printf("x,y:%d,%d\n", curx, cury);
			Vec3b *p1 = srcimage.ptr<Vec3b>(curx, cury);
			Vec3b *p2;
			//int bNewNeighbour = false;
			if (curx - 1 >= 0) {//North
				p2 = srcimage.ptr<Vec3b>(curx - 1, cury);
				if (FIND_REGION_isSimilar(p1, p2) && matCheck[curx - 1][cury] == false) {
					matCheck[curx - 1][cury] = true;
					stackN[stackSize] = curx - 1;
					stackM[stackSize] = cury;
					stackSize += 1;
				}
			}
			if (cury + 1 < nCols) {//East
				p2 = srcimage.ptr<Vec3b>(curx, cury + 1);
				if (FIND_REGION_isSimilar(p1, p2) && matCheck[curx][cury + 1] == false) {
					matCheck[curx][cury + 1] = true;
					stackN[stackSize] = curx;
					stackM[stackSize] = cury + 1;
					stackSize += 1;
				}
			}
			if (curx + 1 < nRows) {//South
				p2 = srcimage.ptr<Vec3b>(curx + 1, cury);
				if (FIND_REGION_isSimilar(p1, p2) && matCheck[curx + 1][cury] == false) {
					matCheck[curx + 1][cury] = true;
					stackN[stackSize] = curx + 1;
					stackM[stackSize] = cury;
					stackSize += 1;
				}
			}
			if (cury - 1 >= 0) {//West
				p2 = srcimage.ptr<Vec3b>(curx, cury - 1);
				if (FIND_REGION_isSimilar(p1, p2) && matCheck[curx][cury - 1] == false) {
					matCheck[curx][cury - 1] = true;
					stackN[stackSize] = curx;
					stackM[stackSize] = cury - 1;
					stackSize += 1;
				}
			}
		}

		break;
	default:
		break;
	}
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

bool CImageProcessingEngine::FIND_REGION_isSimilar(Vec3b *p1, Vec3b *p2) {
	if (p1->val[0] == p2->val[0] && p1->val[1] == p2->val[1] && p1->val[2] == p2->val[2]) {
		return true;
	}
	else {
		return false;
	}
}