AE0434 - Exercise
-----------------------

### Part I - Create an image analysis service ###

Main objective of Part 1 of the exercise is to 

1) implement C++ class(es) to perform the following functions:

* FIND_REGION - accepts an RGB image, an (X, Y) pixel location, and other parameter(s) to control a greedy style algorithm that "flood fills" the nearby pixels in order to find a contiguous patch of pixels that are similar in color. The return value should be some representation of this set of pixels, to be passed to the functions below. Select a reasonable way to control how similar the pixels are; the idea is that regions that a human generally would see as visually similar should be found by your algorithm.

* FIND_PERIMETER - accepts the output of FIND_REGION and outputs contiguous pixels that border the region. These pixels should be a subset of pixels returned by FIND_REGION.

* DISPLAY_IMAGE - provides a method for displaying loaded RGB images.

* DISPLAY_PIXELS - provides a method for displaying an image representation of the results from the above FIND_* operators.

* SAVE_PIXELS - accepts the output from FIND_* and saves the image to a file.

and
	
2) develop a command-line tool to test these functions.


### 1) Code Description: ###

Two classes are implemented and are provided in the CImageProcessingEngine.h/.cpp files:

i) class CFIND_Results

This class implements an object to capture the results from a FIND_ operation, which returns a list of	 pixel coordinates.
The class mainly uses a 1-D array to store the pixel coordinates, but also have a redundant 2-D matrix for quicker search performance (e.g., for FIND_PERIMETER function).
This class also provides the capability to convert the stored pixel coordinate information to image representation.  In particular, a black and white image is returned 
where white pixels denote the pixels returned by the FIND_ operation, and black otherwise.

Note: there is currently a deficiency/inconvenience in the code due to the use of a static 2-D matrix.  In particular, the CFIND_Results object needs to be sized (using the 'resize' function)
to the image size prior to being passed to the FIND_ operation, and cannot be resized during the FIND_ operation.

In the future, the code will be updated to dynamically resize the 2-D matrix as new pixel coordinates are added. 

ii) class CImageProcessingEngine

This class implements all of the above image analysis functions.  The following describes the algorithms behind the functions.

* FIND_REGION - accepts an image and (x,y) coordinate, and returns a CFIND_Results object that stores all coordinates of pixels similar to the original (x,y) pixel.

	The algorithm starts at (x,y) and visits its immediate neighbouring pixels.  If there is a neighbour that is 'similar' and hasn't been visited yet, then it is added to a local
	stack.  The algorithm repeats until the stack is empty.

	'Similiarity' is determined by the class' private function 'FIND_REGION_isSimilar', which (for now) claims two pixels to be 'similar' if their RGB colors are the same.
	
	Note that this function can easily be modified with other 'similarity' criteria such as shade, grayscale, etc.
	
	Also, the similarity function  is currently a private member of the class, but a future improvement can be to pass function pointers to allow 
	other code to provide their own similarity functions.
	
* FIND_PERIMETER - accepts a CFIND_Results object (i.e., generated from FIND_REGION), and returns another CFIND_Results object which stores pixel coordinates of "perimeter pixels".

	The algorithm goes through each pixel in the CFIND_Results object, checks each pixel's surrounding neighbours, and flags a pixel as a 
	perimeter pixel if either 

	i) the pixel has a neighbour that is not similar (i.e., neighbour pixel is not in the CFIND_Results list); and/or

	ii) the pixel is on the edge of the image (i.e., has a neighbour that is out of bounds).


* DISPLAY_IMAGE - a short function that calls openCV imshow.

* DISPLAY_PIXELS - obtains an image presentation from CFIND_Results, and calls openCV imshow.

* SAVE_PIXELS - a short function which calls openCV imwrite.


### 2) Command-line tool ###

The command-line tool is implemented in viewtester.cpp.  

* To run,

```
$ ./view
```

* The command-line tool supports the following commands:

	* quit				- exit proram
	* load				- load image
	* show_image		- display current image (calls DISPLAY_IMAGE)
	* save				- save current image (calls SAVE_IMAGE) to "output.png" (see bug report below)
	* find_region		- calls FIND_REGION on the current image 			
	* find_perimeter 	- calls FIND_PERIMETER on the current image
	
	Note: the program always has only one image in memory.  Loading another image will remove the previous image.
	
	Bug report: the save command currently cannot save to the filename entered by the user, and will always default to 'output.png'
	
	