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
	if (isPointInResults(p) < 0) {
		results.push_back(p);
		mat[p.x][p.y] = results.size() - 1;
	}
}

cv::Point CFIND_Results::getPoint(int ii) {
	return results.at(ii);
}

void CFIND_Results::clear() {
	results.clear();
	int nRows = mat.size();
	int nCols = mat[0].size();
	//mat.clear();
	//for (int ii = 0; ii < mat.size(); ii++)
	//	for (int jj = 0; jj < mat[ii].size(); jj++)
	//		mat[ii][jj] = false;
	mat.resize(nRows, std::vector<int>(nCols, -1));

}

// Note: search through the main 'results' vector is slow.  Added 2D matrix.
int CFIND_Results::isPointInResults(cv::Point p) {
	//for (int ii = 0; ii < results.size(); ii++) {
	//	if (results.at(ii).x == p.x && results.at(ii).y == p.y)
	//		return true;
	//}
	if (p.x < 0 || p.x >= mat.size() || p.y < 0 || p.y >= mat[0].size()) return -1;
	return mat[p.x][p.y];
}

int CFIND_Results::numPoints() {
	return results.size();
}

cv::Point CFIND_Results::size() {
	return Point(mat.size(), mat[0].size());
}
// resize 2D matrix and perform a clear
void CFIND_Results::resize(int nRows,int nCols) {
	//mat.clear();
	//mat.resize(nRows);
	//for (int ii = 0; ii < nRows; ii++) mat[ii].resize(nCols,false);
	mat.resize(nRows, std::vector<int>(nCols, -1));
	results.clear();
}

void CFIND_Results::convertVectorToImage(std::vector<cv::Point> v, cv::Mat &image) {
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

// create black and white image based pixel list
void CFIND_Results::convertToImage(cv::Mat &image) {
	convertVectorToImage(results, image);
	return;
	//Vec3b *p;
	//// clear image: make all pixels black
	//for (int ii = 0; ii < image.rows; ii++)
	//	for (int jj = 0; jj < image.cols; jj++) {
	//		p = image.ptr<Vec3b>(ii, jj);
	//		p->val[0] = 0;
	//		p->val[1] = 0;
	//		p->val[2] = 0;
	//	}
	//
	//// make all pixels in list white
	//for (int ii = 0; ii < results.size(); ii++) {
	//	//printf("(x,y)=(%d,%d)\n", results.at(ii).x, results.at(ii).y);
	//	p = image.ptr<Vec3b>(results.at(ii).x, results.at(ii).y);
	//	p->val[0] = 255;
	//	p->val[1] = 255;
	//	p->val[2] = 255;
	//}
	//return;
}


bool CFIND_Results::loadFromImage(cv::Mat &image) {
	resize(image.rows, image.cols);
	Vec3b *p;
	// clear image: make all pixels black
	for (int ii = 0; ii < image.rows; ii++)
		for (int jj = 0; jj < image.cols; jj++) {
			p = image.ptr<Vec3b>(ii, jj);
			if (p->val[0] == 255 && p->val[1] == 255 && p->val[2] == 255) {
				addPoint(cv::Point(ii, jj));
			}
			else if (p->val[0] == 0 && p->val[1] == 0 && p->val[2] == 0) {
			}
			else {
				// not a CFIND_Results image
				return false;
			}
		}
}

void CFIND_Results::display_pixels(std::vector<cv::Point> v, std::string const &win_name, cv::String const &filename) {

	Mat image(size().x, size().y, CV_8UC3, Scalar(0, 0, 0));

	convertVectorToImage(v,image);
	namedWindow(win_name, WINDOW_AUTOSIZE);
	imshow(win_name, image);
	waitKey(0);

	imwrite(filename, image);

}

void CFIND_Results::copyTo(CFIND_Results &dst) {
	dst.resize(mat.size(), mat[0].size());
	for (int ii = 0; ii < results.size(); ii++)
		dst.addPoint(results[ii]);
}

void CFIND_Results::sortAlongPath() {
	
	int ii_origin = 0;
	for (int ii = 1 ; ii < results.size(); ii++) {
		if (results[ii].x < results[ii_origin].x)
			ii_origin = ii;
		else if (results[ii].x == results[ii_origin].x && results[ii].y < results[ii_origin].y)
			ii_origin = ii;
	}



	std::vector<bool> check(results.size(), false);

	std::vector<cv::Point>newResults;
	newResults.clear();
	//results.swap()
	newResults.push_back(results[ii_origin]);

	//check[ii_origin] = true;

	int count = results.size()-1;
	int count2 = 0;
	while (count > 0) {
		count--;

		std::vector<cv::Point> idx(8);
		idx[0] = cv::Point(0, -1);
		idx[1] = cv::Point(1, 0);
		idx[2] = cv::Point(0, +1);
		idx[3] = cv::Point(-1, 0);
		idx[4] = cv::Point(-1, -1);
		idx[5] = cv::Point(1, -1);
		idx[6] = cv::Point(1, +1);
		idx[7] = cv::Point(-1, +1);
		//idx[0] = cv::Point(-1, -1);
		//idx[1] = cv::Point(0, -1);
		//idx[2] = cv::Point(1, -1);
		//idx[3] = cv::Point(1, 0);
		//idx[4] = cv::Point(1, 1);
		//idx[5] = cv::Point(0, 1);
		//idx[6] = cv::Point(-1, 1);
		//idx[7] = cv::Point(-1, 0);

		cv::Point curp = newResults.back();
		//for(int jj=-1;jj<=1;jj++)
		//	for (int ii = -1; ii <= 1; ii++) {
		int ii, jj;
		for(int kk=0;kk<8;kk++){
			ii = idx[kk].x;
			jj = idx[kk].y;
				if (ii == 0 && jj == 0) continue;
				cv::Point neighbourp = cv::Point(curp.x + ii, curp.y + jj);
				//printf("#%d:(%d,%d),(%d,%d)\n", count,curp.x, curp.y, curp.x + ii, curp.y + jj);
				if (isPointInResults(neighbourp)>=0) {
					if (check[isPointInResults(neighbourp)] == false) {
						newResults.push_back(neighbourp);
						check[isPointInResults(neighbourp)] = true;
						break;
						if (neighbourp.x == newResults[0].x && neighbourp.y == newResults[0].y) {
							printf("break\n");
							break;
						}
					}
				}
		}
	}
	printf("size of results, newresults: %d,%d", results.size(), newResults.size());
	//display_pixels(results, "CFIND_results: results","temp1.png");
	//display_pixels(newResults, "CFIND_results: new results","temp2.png");
	results = newResults;
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
void CImageProcessingEngine::FIND_REGION(cv::Mat &srcimage, CFIND_Results &regionResults, int x, int y) {

	int nRows = srcimage.rows;
	int nCols = srcimage.cols;

	regionResults.clear();

	// initialize stack variables
	std::vector<cv::Point>stack;
	stack.clear();

	//	int count = 0;
	cv::Point p;
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

double CImageProcessingEngine::FIND_SMOOTH_PERIMETER_isAngleSmallerThan(cv::Point2d a, cv::Point2d b, double angle_threshold) {
	return (a.x*b.x + a.y*b.y) / sqrt(a.x*a.x + a.y*a.y) / sqrt(b.x*b.x + b.y*b.y) > cos(angle_threshold);
}

double CImageProcessingEngine::FIND_SMOOTH_PERIMETER_distance(cv::Point2d p1, cv::Point2d p2) {
	return sqrt((p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y));
}

double CImageProcessingEngine::FIND_SMOOTH_PERIMETER_distanceToBezierCurve(cv::Point2d pi, cv::Point2d c0, cv::Point2d c1, cv::Point2d c2, cv::Point2d c3 ) {
	int numStep = max(abs(c0.x-c3.x),abs(c0.y-c3.y))*10;

	double dist = FIND_SMOOTH_PERIMETER_distance(pi,c0);
	for (int ii = 1; ii <= numStep; ii++) {
		double t = ii / numStep;
		/*Point2d bezierPt((1 - t)*((1 - t)*p0.x + t*p1.x) + t*((1 - t)*p1.x + t*p2.x),
						 (1 - t)*((1 - t)*p0.y + t*p1.y) + t*((1 - t)*p1.y + t*p2.y));*/
		Point2d bezierPt(c0.x*pow(1-t,3) + 3*c1.x*t*pow(1-t,2) + 3*c2.x*t*t*(1-t) + c3.x*t*t*t,
			c0.y*pow(1 - t, 3) + 3 * c1.y*t*pow(1 - t, 2) + 3*c2.y*t*t*(1 - t) + c3.y*t*t*t);
		double tempDist = FIND_SMOOTH_PERIMETER_distance(pi, bezierPt);
		if (tempDist < dist) dist = tempDist;
	}
	return dist;

}

//double CImageProcessingEngine::FIND_SMOOTH_PERIMETER_dataFittingCost(CFIND_Results &results, int i1, int i2) {
double CImageProcessingEngine::FIND_SMOOTH_PERIMETER_dataFittingCost(std::vector<cv::Point2d>data){
	double cost = 0;
	//cv::Point2d p1 = Point2d(results.getPoint(i1).x, results.getPoint(i1).y);
	//cv::Point2d p2 = Point2d(results.getPoint(i2).x, results.getPoint(i2).y);
	//cv::Point2d p3 = Point2d(results.getPoint(i3).x, results.getPoint(i3).y);
	std::vector<double> datax;
	std::vector<double> datay;
	datax.clear();
	datay.clear();

	for (int ii = 0; ii < data.size(); ii++) {
		datax.push_back(data[ii].x);
		datay.push_back(data[ii].y);
	}

	Vec4d cx = FIND_SMOOTH_PERIMETER_cubicBezierCurveFitting(datax);
	Vec4d cy = FIND_SMOOTH_PERIMETER_cubicBezierCurveFitting(datay);

	for (int ii = 0; ii < data.size(); ii++) {
		double dist = FIND_SMOOTH_PERIMETER_distanceToBezierCurve(data[ii], Point2d(cx[0],cy[0]), Point2d(cx[1], cy[1]), Point2d(cx[2], cy[2]), Point2d(cx[3], cy[3]));
		cost += dist*dist;
	}
	return cost;
}

cv::Vec4d CImageProcessingEngine::FIND_SMOOTH_PERIMETER_cubicBezierCurveFitting(std::vector<double> data){
	Matx44d A;
	Vec4d b;
	std::vector<double> t;

	t.clear();
	for (int ii = 0; ii < data.size(); ii++) t.push_back((double)ii / (data.size() - 1));

	// assume data is sorted in order, which is true;
	for (int ii = 0; ii < 4; ii++) {
		for (int jj = 0; jj < 4; jj++) {
			//A.at<double>(ii,jj) = 0;
			A(ii,jj) = 0;
		}
		b[ii] = 0;
	}

	for (int ii = 0; ii < data.size(); ii++) {
		//A.at<double>(0, 0) = A.at<double>(0, 0) + 2 * pow((t[ii] - 1), 6);
		//A.at<double>(0, 1) = A.at<double>(0, 1) -6 * t[ii] * pow((t[ii] - 1), 5);
		//A.at<double>(0, 2) = A.at<double>(0, 2) + 6 * pow(t[ii], 2) * pow((t[ii] - 1), 4);
		//A.at<double>(0, 3) = A.at<double>(0, 3) -2 * pow(t[ii], 3) * pow((t[ii] - 1), 3);
		//A.at<double>(1, 0) = A.at<double>(1, 0) -6 * t[ii] * pow((t[ii] - 1), 5);
		//A.at<double>(1, 1) = A.at<double>(1, 1) + 18 * pow(t[ii], 2) * pow((t[ii] - 1), 4);
		//A.at<double>(1, 2) = A.at<double>(1, 2) -18 * pow(t[ii], 3) * pow((t[ii] - 1), 3);
		//A.at<double>(1, 3) = A.at<double>(1, 3) + 6 * pow(t[ii], 4) * pow((t[ii] - 1), 2);
		//A.at<double>(2, 0) = A.at<double>(2, 0) + 6 * pow(t[ii], 2) * pow((t[ii] - 1), 4);
		//A.at<double>(2, 1) = A.at<double>(2, 1) -18 * pow(t[ii], 3) * pow((t[ii] - 1), 3);
		//A.at<double>(2, 2) = A.at<double>(2, 2) + 18 * pow(t[ii], 4) * pow((t[ii] - 1), 2);
		//A.at<double>(2, 3) = A.at<double>(2, 3) -6 * pow(t[ii], 5) * (t[ii] - 1);
		//A.at<double>(3, 0) = A.at<double>(3, 0) -2 * pow(t[ii], 3) * pow((t[ii] - 1), 3);
		//A.at<double>(3, 1) = A.at<double>(3, 1) + 6 * pow(t[ii], 4) * pow((t[ii] - 1), 2);
		//A.at<double>(3, 2) = A.at<double>(3, 2) -6 * pow(t[ii], 5) * (t[ii] - 1);
		//A.at<double>(3, 3) = A.at<double>(3, 3) + 2 * pow(t[ii], 6);
		
		//A[0][0] = A[0][0] + 2 * pow((t[ii] - 1), 6);
		//A[0][1] = A[0][1] - 6 * t[ii] * pow((t[ii] - 1), 5);
		//A[0][2] = A[0][2] + 6 * pow(t[ii], 2) * pow((t[ii] - 1), 4);
		//A[0][3] = A[0][3] - 2 * pow(t[ii], 3) * pow((t[ii] - 1), 3);
		//A[1][0] = A[1][0] - 6 * t[ii] * pow((t[ii] - 1), 5);
		//A[1][1] = A[1][1] + 18 * pow(t[ii], 2) * pow((t[ii] - 1), 4);
		//A[1][2] = A[1][2] - 18 * pow(t[ii], 3) * pow((t[ii] - 1), 3);
		//A[1][3] = A[1][3] + 6 * pow(t[ii], 4) * pow((t[ii] - 1), 2);
		//A[2][0] = A[2][0] + 6 * pow(t[ii], 2) * pow((t[ii] - 1), 4);
		//A[2][1] = A[2][1] - 18 * pow(t[ii], 3) * pow((t[ii] - 1), 3);
		//A[2][2] = A[2][2] + 18 * pow(t[ii], 4) * pow((t[ii] - 1), 2);
		//A[2][3] = A[2][3] - 6 * pow(t[ii], 5) * (t[ii] - 1);
		//A[3][0] = A[3][0] - 2 * pow(t[ii], 3) * pow((t[ii] - 1), 3);
		//A[3][1] = A[3][1] + 6 * pow(t[ii], 4) * pow((t[ii] - 1), 2);
		//A[3][2] = A[3][2] - 6 * pow(t[ii], 5) * (t[ii] - 1);
		//A[3][3] = A[3][3] + 2 * pow(t[ii], 6);

		A(0, 0) = A(0, 0) + 2 * pow((t[ii] - 1), 6);
		A(0, 1) = A(0, 1) -6 * t[ii] * pow((t[ii] - 1), 5);
		A(0, 2) = A(0, 2) + 6 * pow(t[ii], 2) * pow((t[ii] - 1), 4);
		A(0, 3) = A(0, 3) -2 * pow(t[ii], 3) * pow((t[ii] - 1), 3);
		A(1, 0) = A(1, 0) -6 * t[ii] * pow((t[ii] - 1), 5);
		A(1, 1) = A(1, 1) + 18 * pow(t[ii], 2) * pow((t[ii] - 1), 4);
		A(1, 2) = A(1, 2) -18 * pow(t[ii], 3) * pow((t[ii] - 1), 3);
		A(1, 3) = A(1, 3) + 6 * pow(t[ii], 4) * pow((t[ii] - 1), 2);
		A(2, 0) = A(2, 0) + 6 * pow(t[ii], 2) * pow((t[ii] - 1), 4);
		A(2, 1) = A(2, 1) -18 * pow(t[ii], 3) * pow((t[ii] - 1), 3);
		A(2, 2) = A(2, 2) + 18 * pow(t[ii], 4) * pow((t[ii] - 1), 2);
		A(2, 3) = A(2, 3) -6 * pow(t[ii], 5) * (t[ii] - 1);
		A(3, 0) = A(3, 0) -2 * pow(t[ii], 3) * pow((t[ii] - 1), 3);
		A(3, 1) = A(3, 1) + 6 * pow(t[ii], 4) * pow((t[ii] - 1), 2);
		A(3, 2) = A(3, 2) -6 * pow(t[ii], 5) * (t[ii] - 1);
		A(3, 3) = A(3, 3) + 2 * pow(t[ii], 6);

		b[0] = b[0] - 2 * data[ii] * pow((t[ii] - 1), 3);
		b[1] = b[1] + 6 * data[ii] * t[ii] * pow((t[ii] - 1), 2);
		b[2] = b[2] - 6 * data[ii] * pow(t[ii],2) * (t[ii] - 1);
		b[3] = b[3] + 2 * data[ii] * pow(t[ii],3) ;
	}

	Matx44d A_inv = A.inv();

	Vec4d c;
	for (int ii = 0; ii < 4; ii++) {
		//c[ii] = A_inv.at<double>(ii, 0)*b[0] + A_inv.at<double>(ii, 1)*b[1] + A_inv.at<double>(ii, 2)*b[2] + A_inv.at<double>(ii, 3)*b[3];
		//c[ii] = A_inv[ii][0]*b[0] + A_inv[ii][1]*b[1] + A_inv[ii][2]*b[2] + A_inv[ii][3]*b[3];
		c[ii] = A_inv(ii, 0)*b[0] + A_inv(ii, 1)*b[1] + A_inv(ii, 2)*b[2] + A_inv(ii, 3)*b[3];
	}

	return c;
}


//void CImageProcessingEngine::FIND_SMOOTH_PERIMETER(CFIND_Results &perimeterResults, CFIND_Results &smoothPerimeterResults) {
//	smoothPerimeterResults.resize(perimeterResults.size().x, perimeterResults.size().y);
//	perimeterResults.sortAlongPath();
//	//printf("Not implemented.\n");
//	//perimeterResults.copyTo(smoothPerimeterResults);
//
//	std::vector<cv::Point> vertices;
//	vertices.clear();
//	vertices.push_back(perimeterResults.getPoint(0));
//	vertices.push_back(perimeterResults.getPoint(1));
//
//	for (int ii = 1; ii < perimeterResults.numPoints(); ii++) {
//		if (vertices.size() > 2) {
//			while (vertices.size() > 2 &&
//				FIND_SMOOTH_PERIMETER_isAngleSmallerThan(
//					Point2d(vertices[vertices.size() - 1].x - vertices[vertices.size() - 2].x,
//					vertices[vertices.size() - 1].y - vertices[vertices.size() - 2].y),
//					Point2d(perimeterResults.getPoint(ii).x - vertices[vertices.size() - 1].x,
//					perimeterResults.getPoint(ii).y - vertices[vertices.size() - 1].y),
//					45 * 3.14159265 / 180.0)
//				){
//				vertices.pop_back();
//			}
//			vertices.push_back(perimeterResults.getPoint(ii));
//		}
//		else {
//			vertices.push_back(perimeterResults.getPoint(ii));
//		}
//	}
//	printf("vertices size:%d\n", vertices.size());
//	smoothPerimeterResults.clear();
//	for (int ii = 0; ii < vertices.size(); ii++) {
//		smoothPerimeterResults.addPoint(vertices[ii]);
//	}
//}

void CImageProcessingEngine::FIND_SMOOTH_PERIMETER(CFIND_Results &perimeterResults, CFIND_Results &smoothPerimeterResults) {
	smoothPerimeterResults.resize(perimeterResults.size().x, perimeterResults.size().y);
	perimeterResults.sortAlongPath();
	//printf("Not implemented.\n");
	//perimeterResults.copyTo(smoothPerimeterResults);

	std::vector<cv::Point> vertices;
	vertices.clear();
	vertices.push_back(perimeterResults.getPoint(0));
	vertices.push_back(perimeterResults.getPoint(1));

	for (int ii = 1; ii < perimeterResults.numPoints(); ii++) {
		if (vertices.size() > 2) {
			while (vertices.size() > 2 &&
				FIND_SMOOTH_PERIMETER_isAngleSmallerThan(
					Point2d(vertices[vertices.size() - 1].x - vertices[vertices.size() - 2].x,
						vertices[vertices.size() - 1].y - vertices[vertices.size() - 2].y),
					Point2d(perimeterResults.getPoint(ii).x - vertices[vertices.size() - 1].x,
						perimeterResults.getPoint(ii).y - vertices[vertices.size() - 1].y),
					45 * 3.14159265 / 180.0)
				) {
				vertices.pop_back();
			}
			vertices.push_back(perimeterResults.getPoint(ii));
		}
		else {
			vertices.push_back(perimeterResults.getPoint(ii));
		}
	}
	printf("vertices size:%d\n", vertices.size());
	smoothPerimeterResults.clear();
	for (int ii = 0; ii < vertices.size(); ii++) {
		smoothPerimeterResults.addPoint(vertices[ii]);
	}

	//std::vector<double> tempx;
	//std::vector<double> tempy;
	//tempx.clear();
	//tempy.clear();
	//for (int ii = 0; ii < 20; ii++) {
	//	tempx.push_back(perimeterResults.getPoint(ii).x);
	//	tempy.push_back(perimeterResults.getPoint(ii).y);
	//}

	//Vec4d cx, cy;
	//cx = FIND_SMOOTH_PERIMETER_cubicBezierCurveFitting(tempx);
	//printf("Cx:%f,%f,%f,%f\n", cx[0], cx[1], cx[2], cx[3]);
	//cy = FIND_SMOOTH_PERIMETER_cubicBezierCurveFitting(tempy);
	//printf("Cy:%f,%f,%f,%f\n", cy[0], cy[1], cy[2], cy[3]);

	std::vector<Point2d> temp;
	temp.clear();
	for (int ii = 0; ii < 20; ii++) {
		temp.push_back(perimeterResults.getPoint(ii));
	}
	double cost = FIND_SMOOTH_PERIMETER_dataFittingCost(temp);
	printf("cost:%f\n", cost);

}

void CImageProcessingEngine::DISPLAY_IMAGE(const cv::Mat &image, std::string const &win_name) {
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

void CImageProcessingEngine::SAVE_PIXELS(CFIND_Results &results, cv::String filename) {
	Mat image(results.size().x, results.size().y, CV_8UC3, Scalar(0, 0, 0));

	results.convertToImage(image);
	
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