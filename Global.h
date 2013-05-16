#include <opencv/cv.h>
#include <opencv/cxcore.h>
#include <opencv/highgui.h>
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <vector>

using namespace cv;

extern int ch;

extern Mat A;
extern Mat B;

extern double height;
extern char text[5];

extern Mat descriptors_1, descriptors_2;
extern vector<KeyPoint> keypoints_1, keypoints_2;

extern FastFeatureDetector detector;
extern BriefDescriptorExtractor ex;

extern Mat img_matches, leftImg, rightImg, img_left_remap, img_right_remap;

extern int i;
extern double baseline_mm, focal_length_mm, disparity_pixel, sensor_pixel_per_mm, focal_length_pixel;

extern float distance_mm;
extern int obcount;

extern Mat h;
extern Mat rmap[2][2];
extern FileStorage fs;

int initialize();
