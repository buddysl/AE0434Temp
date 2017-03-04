#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include "CImageProcessingEngine.h"

#include "opencv2/opencv.hpp"

using namespace cv;

/*  Main command-line program.  Supported commands:

	quit		- exit proram
	load		- load image
	show_image	- display image (calls DISPLAY_IMAGE)
	save_pixels	- save current image (calls SAVE_IMAGE)
	find_region	- calls FIND_REGION on the current image in memory.  			
	find_perimeter - calls FIND_PERIMETER on the current image in memory
	
*/

int main(int argc, char **argv) {

	printf("Program started\n\n");
	char s[256];

	CImageProcessingEngine ipengine;
	Mat image;
	CFIND_Results regionResults, perimeterResults, smoothPerimeterResults;

	bool alwaysShowImage = true;
	bool loadedImage = false,  haveRegionResults = false, havePerimeterResults = false, haveSmoothPerimeterResults = false;

	while (1 == 1) {
		printf("Enter input: ");
		scanf("%s", s);
		if (strcmp(s, "quit") == 0) {
			break;
		}
		else if (strcmp(s, "load_image") == 0) {		// load file
			printf("Enter filename: ");
			scanf("%s", s);

			cv::String filename;
			filename = s;
			image = imread(filename, 1);

			if (!image.data) {
				printf("No image data (warning: OpenCV recognize files by extensions)\n");
				continue;
			}
			loadedImage = true;
			regionResults.resize(image.rows, image.cols);	// need results object to be pre-sized to image size
			perimeterResults.resize(image.rows, image.cols); // need results object to be pre-sized to image size
			smoothPerimeterResults.resize(image.rows, image.cols); // need results object to be pre-sized to image size

			printf("Image loaded.  Size: %d x %d\n", image.rows, image.cols);
			if(alwaysShowImage) {
				printf("Press any key on image to continue...\n");
				ipengine.DISPLAY_IMAGE(image, "Initial image");
			}
		}
		else if (strcmp(s, "load_pixels") == 0) {
			int pixelset = -1;
			printf("Which set [0-find_region, 1-find_perimeter, 2-find_smooth_perimeter]: ");
			scanf("%d", &pixelset);

			if (pixelset < 0 || pixelset>2) {
				printf("Invalid selection.\n");
				continue;
			}

			printf("Enter filename: ");
			scanf("%s", s);

			cv::String filename;
			filename = s;

			Mat tempImage;
			tempImage = imread(filename, 1);

			if (!tempImage.data) {
				printf("No image data (warning: OpenCV recognize files by extensions)\n");
				continue;
			}
			switch (pixelset) {
			case 0:
				if (regionResults.loadFromImage(tempImage))
				{
					printf("Results from FIND_REGION loaded.\n");
					haveRegionResults = true;
					if (alwaysShowImage) {
						printf("Press any key on image to continue...\n");
						ipengine.DISPLAY_PIXELS(regionResults, "Find_region Results");
					}
				}
				else
					printf("Failed to convert FIND_REIONG results.\n");
				break;
			case 1:
				if (perimeterResults.loadFromImage(tempImage))
				{
					printf("Results from FIND_PERIMETER loaded.\n");
					havePerimeterResults = true;
					if (alwaysShowImage) {
						printf("Press any key on image to continue...\n");
						ipengine.DISPLAY_PIXELS(perimeterResults, "Find_perimeter Results");
					}
				}
				else
					printf("Failed to convert FIND_PERIMETER results.\n");
				break;
			case 2:
				if (smoothPerimeterResults.loadFromImage(tempImage))
				{
					printf("Results from FIND_SMOOTH_PERIMETER loaded.\n");
					haveSmoothPerimeterResults = true;
					if (alwaysShowImage) {
						printf("Press any key on image to continue...\n");
						ipengine.DISPLAY_PIXELS(smoothPerimeterResults, "Find_smooth_perimeter Results");
					}
				}
				else
					printf("Failed to convert FIND_SMOOTH_PERIMETER results.\n");
				break;
			default:
				printf("Invalid selection.\n");
				break;
			}

		}
		else if (strcmp(s, "show_image") == 0) {	// display image
			if (!loadedImage)
				printf("No image loaded.  Use load command.\n");
			else{
				printf("Press any key on image to continue...\n");
				ipengine.DISPLAY_IMAGE(image,"Current image");
			}
		}
		else if (strcmp(s, "save_pixels") == 0) {
			int pixelset = -1;
			printf("Which set [0-find_region, 1-find_perimeter, 2-find_smooth_perimeter]: ");
			scanf("%d", &pixelset);
			
			switch (pixelset) {
			case 0: 
				if (!haveRegionResults) {
					printf("No find_region results found.\n");
					break;
				}
				ipengine.SAVE_PIXELS(regionResults, "region.png"); //bug: cannot use filename read from user
				printf("Saved.\n");
				break;
			case 1:
				if (!havePerimeterResults) {
					printf("No find_perimeter results found.\n");
					break;
				}
				ipengine.SAVE_PIXELS(perimeterResults, "perimeter.png"); //bug: cannot use filename read from user
				printf("Saved.\n");
				break;
			case 2:
				if (!haveSmoothPerimeterResults) {
					printf("No find_smooth_perimeter results found.\n");
					break;
				}
				ipengine.SAVE_PIXELS(smoothPerimeterResults, "smooth.png"); //bug: cannot use filename read from user
				printf("Saved.\n");
				break;
			default: 
				printf("Invalid selection.\n");
				break;
			}

			//printf("Enter filename: ");
			//scanf("%s", s);

			//cv::String filename;
			//ipengine.SAVE_PIXELS(image, "output.png"); //bug: cannot use filename read from user
			//printf("Saved.\n");
		}
		else if (strcmp(s, "find_region") == 0) {
			int x, y;

			if (!loadedImage) {
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
			haveRegionResults = true;
	
			if (alwaysShowImage) {	
				printf("Press any key on image to continue...\n");
				ipengine.DISPLAY_PIXELS(regionResults, "Find_region Results");
			}
		}
		else if (strcmp(s, "find_perimeter") == 0) {
			if (!haveRegionResults) {
				printf("No results from FIND_REGION.  Please run find_region, or load previous results.\n");
				continue;
			}
			ipengine.FIND_PERIMETER(regionResults, perimeterResults);
			havePerimeterResults = true;
			if (alwaysShowImage) {
				printf("Press any key on image to continue...\n");
				ipengine.DISPLAY_PIXELS(perimeterResults,  "Find_perimeter Results");

			}
		}
		else if (strcmp(s, "find_smooth_perimeter") == 0) {
			if (!havePerimeterResults) {
				printf("No results from FIND_PERIMETER.  Please run find_perimeter, or load previous results.\n");
				continue;
			}
			ipengine.FIND_SMOOTH_PERIMETER(perimeterResults, smoothPerimeterResults);
			haveSmoothPerimeterResults = true;
			if (alwaysShowImage) {
				printf("Press any key on image to continue...\n");
				ipengine.DISPLAY_PIXELS(smoothPerimeterResults, "Find_smooth_perimeter Results");
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

