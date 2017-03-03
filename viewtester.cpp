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
	bool alwaysShowImage = true;

	while (1 == 1) {
		printf("Enter input: ");
		scanf("%s", &s);
		if (strcmp(s, "quit") == 0) {
			break;
		}
		else if (strcmp(s, "load") == 0) {
			printf("Enter filename: ");
			scanf("%s", &s);

			cv::String filename;
			filename = s;
			image = imread(filename, 1);

			if (!image.data) {
				printf("No image data (warning: OpenCV recognize files by extensions)\n");
				break;
			}
			if(alwaysShowImage) ipengine.DISPLAY_IMAGE(image, "Initial image");
		}
		else if (strcmp(s, "show_image") == 0) {
			if (!image.data)
				printf("No image loaded.  Use load command.\n");
			else
				ipengine.DISPLAY_IMAGE(image,"Current image");
		}
		else if (strcmp(s, "save") == 0) {
			printf("Enter filename: ");
			scanf("%s", &s);

			cv::String filename;
			ipengine.SAVE_PIXELS(image, "output.png"); //bug
			printf("Saved.\n");
		}
		else if (strcmp(s, "find_region") == 0) {

			Mat find_region_image;
			find_region_image = image.clone();
			ipengine.FIND_REGION(image, find_region_image, 1, 1);
			image = find_region_image;
			if (alwaysShowImage) ipengine.DISPLAY_IMAGE(image, "Find_region Results");
		}
		else if (strcmp(s, "find_perimeter") == 0) {

		}
		else if (strcmp(s, "toggle_display_mode") == 0){
			alwaysShowImage = !alwaysShowImage;
			if (alwaysShowImage)
				printf("Display mode: show image.\n");
			else
				printf("Display mode: do not show image.\n");
		}
		else {
			printf("Invalid command. Try again.\n");
		}
	}

	printf("Program End.\n");;
	return 0;
}

