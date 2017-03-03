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
		}

	clear_image(dstimage);

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


			//if (curx - 1 >= 0) {//North
			//	p2 = srcimage.ptr<Vec3b>(curx - 1, cury);
			//	if (FIND_REGION_isSimilar(p1, p2) && matCheck[curx - 1][cury] == false) {
			//		matCheck[curx - 1][cury] = true;
			//		stackN[stackSize] = curx - 1;
			//		stackM[stackSize] = cury;
			//		stackSize += 1;
			//	}
			//}
			//if (cury + 1 < nCols) {//East
			//	p2 = srcimage.ptr<Vec3b>(curx, cury + 1);
			//	if (FIND_REGION_isSimilar(p1, p2) && matCheck[curx][cury + 1] == false) {
			//		matCheck[curx][cury + 1] = true;
			//		stackN[stackSize] = curx;
			//		stackM[stackSize] = cury + 1;
			//		stackSize += 1;
			//	}
			//}
			//if (curx + 1 < nRows) {//South
			//	p2 = srcimage.ptr<Vec3b>(curx + 1, cury);
			//	if (FIND_REGION_isSimilar(p1, p2) && matCheck[curx + 1][cury] == false) {
			//		matCheck[curx + 1][cury] = true;
			//		stackN[stackSize] = curx + 1;
			//		stackM[stackSize] = cury;
			//		stackSize += 1;
			//	}
			//}
			//if (cury - 1 >= 0) {//West
			//	p2 = srcimage.ptr<Vec3b>(curx, cury - 1);
			//	if (FIND_REGION_isSimilar(p1, p2) && matCheck[curx][cury - 1] == false) {
			//		matCheck[curx][cury - 1] = true;
			//		stackN[stackSize] = curx;
			//		stackM[stackSize] = cury - 1;
			//		stackSize += 1;
			//	}
			//}
		}

		break;
	default:
		break;
	}
}

//bool FIND_PERIMETER_sort_helper(cv::Point p1, cv::Point p2) {
//	if (p1.x == p2.x)
//		return p1.y > p2.y;
//	else
//		return p1.x < p2.x;
//}
//
//bool FIND_PERIMETER_isCClockwise(cv::Point p1, cv::Point p2, cv::Point p3) {
//	return (p2.x - p1.x) * (p3.y - p1.y) >= (p2.y - p1.y) * (p3.x - p1.x);
//}

void CImageProcessingEngine::FIND_PERIMETER(cv::Mat &srcimage, cv::Mat &dstimage) {
	
	clear_image(dstimage);

	int nRows = srcimage.rows;
	int nCols = srcimage.cols;

//	std::vector<cv::Point> A;
	//A.clear();

	Vec3b *p;
	Vec3b *p2;
	for(int ii=0;ii<srcimage.rows;ii++)
		for (int jj = 0; jj < srcimage.cols; jj++) {
			p = srcimage.ptr<Vec3b>(ii, jj);
			if (FIND_isPixelSet(p)) {
				//A.push_back(Point(ii, jj));
				int count = 0;
				for (int ii2 = -1; ii2 <= 1; ii2++){
					for (int jj2 = -1; jj2 <= 1; jj2++) {
						if (ii2 == 0 && jj2 == 0) continue;
						if (ii + ii2 < 0 || ii + ii2 >= nRows
							|| jj + jj2 < 0 || jj + jj2 >= nCols) continue;
						p2 = srcimage.ptr<Vec3b>(ii + ii2, jj + jj2);
						if (FIND_isPixelSet(p2))count++;
					}
				}
				if (count < 8) { // border found
					p2 = dstimage.ptr<Vec3b>(ii, jj);
					FIND_SetPixel(p2);
				}
			}
		}
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

void CImageProcessingEngine::DISPLAY_PIXELS() {

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