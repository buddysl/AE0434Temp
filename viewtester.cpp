#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include "CImageProcessingEngine.h"

#include "opencv2/opencv.hpp"

using namespace cv;

/*  Main command-line program.  Supported commands:

	quit		- exit proram
	load_image	- load image
	load pixels	- load pixel file
	show_image	- display image (calls DISPLAY_IMAGE)
	save_pixels	- save current image (calls SAVE_IMAGE)
	find_region	- calls FIND_REGION on the current image in memory.  			
	find_perimeter - calls FIND_PERIMETER on the current 'region' results in memory
	find_smooth_perimeter - calls FIND_SMOOTH_PERIMETER on the current 'region' results in memory
*/

int main(int argc, char **argv) {

	printf("Program started\n\n");
	char s[256];

	CImageProcessingEngine ipengine;
	Mat image;
	CFIND_Results regionResults, perimeterResults, smoothPerimeterResults;

	bool alwaysShowImage = true;
	bool loadedImage = false,  haveRegionResults = false, havePerimeterResults = false, haveSmoothPerimeterResults = false;

	// main loop
	while (1 == 1) {
		printf("Enter input: ");
		scanf("%s", s);
		if (strcmp(s, "quit") == 0) {
			break;
		}

		////////////////////
		// load image file
		else if (strcmp(s, "load_image") == 0) {		
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

		////////////////////
		// load pixel file
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

		////////////////////
		// display image
		else if (strcmp(s, "show_image") == 0) {
			if (!loadedImage)
				printf("No image loaded.  Use load command.\n");
			else{
				printf("Press any key on image to continue...\n");
				ipengine.DISPLAY_IMAGE(image,"Current image");
			}
		}
		
		////////////////////
		// save pixels to file
		else if (strcmp(s, "save_pixels") == 0) {
			int pixelset = -1;
			printf("Which set [0-find_region, 1-find_perimeter, 2-find_smooth_perimeter]: ");
			scanf("%d", &pixelset);

			CFIND_Results* ptresults = nullptr;

			switch (pixelset) {
			case 0: 
				if (!haveRegionResults) {
					printf("No find_region results found.\n"); break;
				}
				ptresults = &regionResults;
				break;
			case 1:
				if (!havePerimeterResults) {
					printf("No find_perimeter results found.\n"); break;
				}
				ptresults = &perimeterResults;				
				break;
			case 2:
				if (!haveSmoothPerimeterResults) {
					printf("No find_smooth_perimeter results found.\n"); break;
				}
				ptresults = &smoothPerimeterResults;
				break;
			default: 
				printf("Invalid selection.\n");
				break;
			}
			if(!(ptresults==nullptr)){
				printf("Enter filename: ");
				scanf("%s", s);

				cv::String filename;
				filename = s;

				ipengine.SAVE_PIXELS(*ptresults, s); //bug: cannot use filename read from user (fixed)
				printf("Saved.\n");
			}

		}

		////////////////////
		// FIND_REGION
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

		////////////////////
		// FIND_PERIMETER
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

		////////////////////
		// FIND_SMOOTH_PERIMETER
		else if (strcmp(s, "find_smooth_perimeter") == 0) {
			if (!haveRegionResults) {
				printf("No results from FIND_REGION.  Please run find_region, or load previous results.\n");
				continue;
			}
			ipengine.FIND_SMOOTH_PERIMETER(regionResults, smoothPerimeterResults);
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

