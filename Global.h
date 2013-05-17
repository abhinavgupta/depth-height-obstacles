#include <opencv/cv.h>
#include <opencv/cxcore.h>
#include <opencv/highgui.h>
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <vector>

/* This header contains externs of all the Global variables that have to be access across all the files*/

using namespace cv;

extern int ch;																							//Camera Height variable

extern Mat A;																							//Homography Matrix at height h
extern Mat B;																							//Homography Matrix at height h

extern double height;																					//Height variable for getHeight()
extern char text[5];																					// For display purposes

extern Mat descriptors_1, descriptors_2;																//Descriptor Matrices
extern vector<KeyPoint> keypoints_1, keypoints_2;														//Keypoint vectors

extern FastFeatureDetector detector;																	//detector function
extern BriefDescriptorExtractor ex;																		//Descriptor function

extern Mat img_matches, leftImg, rightImg, img_left_remap, img_right_remap;								//remap, capture and match matrices

extern int i;																						
extern double baseline_mm, focal_length_mm, disparity_pixel, sensor_pixel_per_mm, focal_length_pixel;	//Camera parameters

extern float distance_mm;																				//Depth variable
extern int obcount;																						//Obstacles count 

extern Mat h;																							//Ground homography matrix
extern Mat rmap[2][2];																					//Calibration matrix
extern FileStorage fs;																					//File handler variable

int initialize();																						//Function to initialize variables
