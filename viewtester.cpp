#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include "CImageProcessingEngine.h"

#include "opencv2/opencv.hpp"

using namespace cv;

/*  Main command-line program.  Supported commands:

	quit		- exit proram
	load		- load image
	show_image	- display image (calls DISPLAY_IMAGE)
	save		- save current image (calls SAVE_IMAGE)
	find_region	- calls FIND_REGION on the current image in memory.  			
	find_perimeter - calls FIND_PERIMETER on the current image in memory
	
*/

int main(int argc, char **argv) {

	printf("Program started\n\n");
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

			regionResults.resize(image.rows, image.cols);	// need results object to be pre-sized to image size
			perimeterResults.resize(image.rows, image.cols); // need results object to be pre-sized to image size

			printf("Image loaded.  Size: %d x %d\n", image.rows, image.cols);
			if(alwaysShowImage) {
				printf("Press any key on image to continue...\n");
				ipengine.DISPLAY_IMAGE(image, "Initial image");
			}
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
			if (!image.data) {
				printf("No image loaded.\n");
				continue;
			}

			printf("Enter filename: ");
			scanf("%s", s);

			cv::String filename;
			ipengine.SAVE_PIXELS(image, "output.png"); //bug: cannot use filename read from user
			printf("Saved.\n");
		}
		else if (strcmp(s, "find_region") == 0) {
			int x, y;

			if (!image.data) {
				printf("No image loaded.\n");
				continue;
			}
			printf("Enter x y: ");
			scanf("%d%d",&x,&y);
			if (x < 0 || x >= image.rows || y < 0 || y >= image.cols) {
				printf("Invalid x y.\n");
				continue;
			}

			ipengine.FIND_REGION(image, regionResults, x, y);
	
			if (alwaysShowImage) {	
				printf("Press any key on image to continue...\n");
				ipengine.DISPLAY_PIXELS(regionResults, image.rows, image.cols, "Find_region Results");
			}
		}
		else if (strcmp(s, "find_perimeter") == 0) {
			if (!image.data) {
				printf("No image loaded.\n");
				continue;
			}
			ipengine.FIND_PERIMETER(regionResults, perimeterResults,image.rows,image.cols);
			if (alwaysShowImage) {
				printf("Press any key on image to continue...\n");
				ipengine.DISPLAY_PIXELS(perimeterResults, image.rows, image.cols, "Find_perimeter Results");

			}
		}
		//else if (strcmp(s, "toggle_display_mode") == 0){
		//	alwaysShowImage = !alwaysShowImage;
		//	if (alwaysShowImage)
		//		printf("Display mode: show image.\n");
		//	else
		//		printf("Display mode: do not show image.\n");
		//}
		else {
			printf("Invalid command. Try again.\n");
		}
		printf("\n");
	}

	printf("\nProgram End.\n");;
	return 0;
}

