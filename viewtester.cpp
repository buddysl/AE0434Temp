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
	CFIND_Results regionResults, perimeterResults;

	bool alwaysShowImage = true;

	while (1 == 1) {
		printf("Enter input: ");
		scanf("%s", s);
		if (strcmp(s, "quit") == 0) {
			break;
		}
		else if (strcmp(s, "load") == 0) {		// load file
			printf("Enter filename: ");
			scanf("%s", s);

			cv::String filename;
			filename = s;
			image = imread(filename, 1);

			if (!image.data) {
				printf("No image data (warning: OpenCV recognize files by extensions)\n");
				continue;
			}
			if(alwaysShowImage) ipengine.DISPLAY_IMAGE(image, "Initial image");
		}
		else if (strcmp(s, "show_image") == 0) {	// display image
			if (!image.data)
				printf("No image loaded.  Use load command.\n");
			else{
				printf("Press any key on image to continue...\n");
				ipengine.DISPLAY_IMAGE(image,"Current image");
			}
		}
		else if (strcmp(s, "save") == 0) {
			printf("Enter filename: ");
			scanf("%s", s);

			cv::String filename;
			ipengine.SAVE_PIXELS(image, "output.png"); //bug: cannot use filename read from user
			printf("Saved.\n");
		}
		else if (strcmp(s, "find_region") == 0) {
			int x, y;
			printf("Enter x y: ");
			scanf("%d%d",&x,&y);

			regionResults.resize(image.rows, image.cols);	// bug: need results object to be pre-sized to image size
			ipengine.FIND_REGION(image, regionResults, x, y);

			//image = find_region_image;
			regionResults.convertToImage(image);
			if (alwaysShowImage) {	
				printf("Press any key on image to continue...\n");
				ipengine.DISPLAY_IMAGE(image, "Find_region Results");

			}
			
			//if (alwaysShowImage) ipengine.DISPLAY_PIXELS(regionResults,image,"Find_region Results");  // bug: not working
		}
		else if (strcmp(s, "find_perimeter") == 0) {

			perimeterResults.resize(image.rows, image.cols); // bug: need results object to be pre-sized to image size
			ipengine.FIND_PERIMETER(regionResults, perimeterResults,image.rows,image.cols);
			perimeterResults.convertToImage(image);
			if (alwaysShowImage) {
				printf("Press any key on image to continue...\n");
				ipengine.DISPLAY_IMAGE(image, "Find_perimeter Results");

			}

			//if (alwaysShowImage) ipengine.DISPLAY_PIXELS(perimeterResults,image,"Find_region Results");  // bug: not working
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

