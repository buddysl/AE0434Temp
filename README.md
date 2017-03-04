AE0434 - Exercise
-----------------------

Part I - Create an image analysis service

Main objective of this exercise is to implement C++ class(es) to perform the following functions:
*FIND_REGION - accepts an RGB image, an (X, Y) pixel location, and other parameter(s) to control a greedy style algorithm that "flood fills" the nearby pixels in order to find a contiguous patch of pixels that are similar in color. The return value should be some representation of this set of pixels, to be passed to the functions below. Select a reasonable way to control how similar the pixels are; the idea is that regions that a human generally would see as visually similar should be found by your algorithm.
*FIND_PERIMETER - accepts the output of FIND_REGION and outputs contiguous pixels that border the region. These pixels should be a subset of pixels returned by FIND_REGION.
*DISPLAY_IMAGE - provides a method for displaying loaded RGB images.
*DISPLAY_PIXELS - provides a method for displaying an image representation of the results from the above FIND_* operators.
*SAVE_PIXELS - accepts the output from FIND_* and saves the image to a file.