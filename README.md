AE0434 - Exercise Introduction
-----------------------

### Part I - Create an image analysis service ###

Main objective of Part I of the exercise is to 

1) implement C++ class(es) to perform the following functions:

* FIND_REGION - accepts an RGB image, an (X, Y) pixel location, and other parameter(s) to control a greedy style algorithm that "flood fills" the nearby pixels in order to find a contiguous patch of pixels that are similar in color. The return value should be some representation of this set of pixels, to be passed to the functions below. Select a reasonable way to control how similar the pixels are; the idea is that regions that a human generally would see as visually similar should be found by your algorithm.

* FIND_PERIMETER - accepts the output of FIND_REGION and outputs contiguous pixels that border the region. These pixels should be a subset of pixels returned by FIND_REGION.

* DISPLAY_IMAGE - provides a method for displaying loaded RGB images.

* DISPLAY_PIXELS - provides a method for displaying an image representation of the results from the above FIND_* operators.

* SAVE_PIXELS - accepts the output from FIND_* and saves the image to a file.

and
	
2) develop a command-line tool to test these functions.

### Part II - Add additional image analysis service ###

Main objective of Part II is to add an addition function to Part I:

* FIND_SMOOTH_PERIMETER - accepts the outupt of FIND_REGION and outputs a perimeter that has been "smoothed":
	

Submitted solution
-----------------------

### 1) Code Description: ###

Two classes are implemented and are provided in the CImageProcessingEngine.h/.cpp files:

i) Class CFIND_Results

This class implements an object to capture and manipulate the list of pixel coordinates returned from a FIND_ operation.
The class is implemented using both a 1-D array, and a redundant 2-D matrix for quicker search performance (e.g., for large images).
This class also provides the capability to store/load the pixel information to and from file.  In particular, the pixel information is stored
as a black and white image, where white pixels denote the pixels returned by the FIND_ operation, and black otherwise.

Note: there is currently a deficiency/inconvenience in the code due to the use of a static 2-D matrix.  In particular, the CFIND_Results object needs to be sized (using the 'resize' function)
to the image size prior to being passed to the FIND_ operation, and cannot be resized during the FIND_ operation. Update: comment obsolete.


ii) Class CImageProcessingEngine

This class implements all of the above FIND_ image analysis functions.  The following describes the algorithms behind the functions.

* FIND_REGION - accepts an image and (x,y) coordinate, and returns a CFIND_Results object that stores all coordinates of pixels similar to the original (x,y) pixel.

	The algorithm starts at (x,y) and visits its immediate neighbouring pixels.  If there is a neighbour that is 'similar' and hasn't been visited yet, then it is added to a local
	stack.  The algorithm repeats until the stack is empty.

	'Similiarity' is determined by the class' private function 'FIND_REGION_isSimilar', which (for now) claims two pixels to be 'similar' if their RGB colors are the same.
	
	Note that this function can easily be modified with other 'similarity' criteria such as shade, grayscale, etc.
	
	Also, the similarity function is currently a private member in the class, but a future improvement can be made to pass function pointers to allow 
	other user code to provide their own similarity functions.
	
* FIND_PERIMETER - accepts a CFIND_Results object (i.e., generated from FIND_REGION), and returns another CFIND_Results object which stores pixel coordinates of "perimeter pixels".

	The algorithm goes through each pixel in the CFIND_Results object, checks each pixel's surrounding neighbours, and flags a pixel as a 
	perimeter pixel if either 

	i) the pixel has a neighbour that is not similar (i.e., neighbour pixel is not in the CFIND_Results list); and/or

	ii) the pixel is on the edge of the image (i.e., has a neighbour that is out of bounds).
	
* FIND_SMOOTH_PERIMETER - similar to the FIND_PERIMETER, except the perimeter has been smooth.

	The algorithm performs the following steps to obtain a smooth contour around the FIND_REGION:
	
	i) Remove small artifacts (i.e., small pixels)
		
		- Algorithm goes through all the pixels returned from FIND_REGION, and for each pixel, counts the number of surrounding neighbours.  If fhe number of neighbors		
		is less than some given threshold, then the pixel will be flagged as a 'small artifact' pixel and removed from FIND_REGION results.
		
	ii) Perform the FIND_PERIMETER to obtain the perimeter of FIND_REGION.
	
	iii) Find the smallest convex hull on the perimeter.
			
		- The algorithm first sorts the pixels and then traverses each pixel in a clockwise order. If a counter clockwise move is made, the algorithm backtracks 
		and one-by-one removes the previous pixels until there is no longer a the counter clockwise move.  
			The algorithm first builds the upper hull, and then repeats for the lower hall.
	
	iv) Curve-fit the convex hull using splines.
	
		- Adapted method from https://www.mathworks.com/matlabcentral/fileexchange/7078-cardinal-spline--catmull-rom--spline.
		
* DISPLAY_IMAGE - a short function that calls openCV imshow.

* DISPLAY_PIXELS - obtains an image presentation from CFIND_Results, and calls openCV imshow.

* SAVE_PIXELS - obtains an image presentation from CFIND_Results, and calls openCV imwrite.


### 2) Command-line tool ###

The command-line tool is implemented in viewtester.cpp.  

* To run,

```
$ ./view
```

* The command-line tool supports the following commands:

	* quit		- exit program
	* load_image	- load image
	* load pixels	- load pixel file from previous FIND_REGION, _PERIMETER, _SMOOTH_PERIMETER
	* show_image	- display image (calls DISPLAY_IMAGE)
	* save_pixels	- save current image to a pixel file (calls SAVE_IMAGE)
	* find_region	- calls FIND_REGION on the current image in memory.  			
	* find_perimeter - calls FIND_PERIMETER on the current 'region' results in memory
	* find_smooth_perimeter - calls FIND_SMOOTH_PERIMETER on the current 'region' results in memory
	
	Note: the program only keeps track of one image in memory.  Loading another image will remove the previous image.
	
	Bug report (fixed): the save command currently cannot save to the filename entered by the user, and will always default to 'output.png'.
	
	
### 3) Test results ###

The following section demonstrates the results of the code.

* Original image: test3.png

	![alt text](/demo_results/test3.png?raw=true)

* Results of FIND_REGION with (100,100).

	![alt text](/demo_results/test3_region.png?raw=true)

* Results of FIND_PERIMETER

	![alt text](/demo_results/test3_perimeter.png?raw=true)

* Results of FIND_SMOOTH_PERIMETER

	|Removed small artifacts |![alt text](/demo_results/test3_smooth_clean.png?raw=true)|

	- Removed small artifacts:
	
	![alt text](/demo_results/test3_smooth_clean.png?raw=true)

		Note: there are still some noticeable small pixels not cleaned.  Bug report: need to either tweak threshold, or an alternative cleaning method.
	
	- Found perimeter:
	
	![alt text](/demo_results/test3_smooth_perimeter.png?raw=true)
	
	- Found convex hull:
	
	![alt text](/demo_results/test3_smooth_convex_hull.png?raw=true)

	- Curve fitting:
	
	![alt text](/demo_results/test3_smooth_spline.png?raw=true)

	