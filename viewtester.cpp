#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include "CImageProcessingEngine.h"

#include "opencv2/opencv.hpp"

using namespace cv;

int main(int argc, char **argv) {

	printf("Program started\n");
	char s[256];

	CImageProcessingEngine ipengine;
	Mat image;

	while (1 == 1) {
		printf("Enter input: ");
		scanf("%s", &s);
		if (strcmp(s, "quit") == 0) {
			break;
		}
		else if (strcmp(s, "display") == 0) {
			printf("Enter filename: ");
			scanf("%s", &s);

			cv::String filename;
			filename = s;
			image = imread(filename, 1);

			if (!image.data) {
				printf("No image data (warning: OpenCV recognize files by extensions)\n");
				break;
			}
			ipengine.DISPLAY_IMAGE(image,"Input");
		}
		else if (strcmp(s, "find_region") == 0) {
			printf("Enter filename: ");
			scanf("%s", &s);

			cv::String filename;
			filename = s;
			image = imread(filename, 1);
			if (!image.data) {
				printf("No image data (warning: OpenCV recognize files by extensions)\n");
				break;
			}
			Mat find_region_image(image);
			ipengine.FIND_REGION(image, find_region_image, 1, 1);
			ipengine.DISPLAY_IMAGE(find_region_image, "FIND REGION RESULTS");
		}
	}

	printf("Program End.\n");;
	return 0;
}

