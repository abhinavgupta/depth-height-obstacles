#ifndef DEPTHHEIGHTCALCULATION_H
#define DEPTHHEIGHTCALCULATION_H
#include "opencv2/opencv.hpp"

/* This header contains the function for depth_height and obstacle detection*/

using namespace cv;

int depth_height_calc(Mat leftImg, Mat rightImg);

#endif
