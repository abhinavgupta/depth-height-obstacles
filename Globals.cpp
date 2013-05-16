#include "Global.h"
#include <opencv/cv.h>
#include <opencv/cxcore.h>
#include <opencv/highgui.h>
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <vector>

using namespace cv;

int ch = 5;

Mat A = Mat(3,1, CV_64F);
Mat B = Mat(3,1, CV_64F);

double height;

char text[5];

VideoCapture cap_left(0), cap_right(1);
Mat img_matches, leftImg, rightImg, img_left_remap, img_right_remap;

int i=0;
double baseline_mm = 120, focal_length_mm = 2.5;
double disparity_pixel,sensor_pixel_per_mm=278.26,focal_length_pixel=640;

Mat h(3,3, CV_64F);

Mat rmap[2][2];
FileStorage fs("rmaps.yml", CV_STORAGE_READ);


int initialize()
{


	h.at<double>(0,0) = 1.0365;
	h.at<double>(0,1) = -0.946;
	h.at<double>(0,2) = 116.715;
	h.at<double>(1,0) = -0.1621;    
	h.at<double>(1,1) = 1.2213;    
	h.at<double>(1,2) = -2.1424;    
	h.at<double>(2,0) = -0.000973;    
	h.at<double>(2,1) = 0.0012;    
	h.at<double>(2,2) = 1;  


	fs["rmap00"]>>rmap[0][0];	
	fs["rmap01"]>>rmap[0][1];
	fs["rmap10"]>>rmap[1][0];
	fs["rmap11"]>>rmap[1][1];
	fs.release();
	
	cap_left.set(CV_CAP_PROP_FRAME_WIDTH, 320); 
	cap_left.set(CV_CAP_PROP_FRAME_HEIGHT, 240);
	cap_right.set(CV_CAP_PROP_FRAME_WIDTH, 320);
	cap_right.set(CV_CAP_PROP_FRAME_HEIGHT, 240);

	return 0;
}


