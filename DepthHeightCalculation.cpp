#include <algorithm>
#include <iostream>
#include <opencv/cv.h>
#include <opencv/cxcore.h>
#include <opencv/highgui.h>
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "Global.h"

using namespace cv;
using namespace std;


vector<KeyPoint> keypoints_1, keypoints_2;
FastFeatureDetector detector(10);

Mat descriptors_1, descriptors_2;
BriefDescriptorExtractor ex;

float distance_mm;
int obcount;

int getHeight(Mat H,int ch, Point i, Point f);	//Function to get height of set of points 

bool myobject (DMatch i,DMatch j)	//Object for using std:sort on the cv::DMatch class
{ 
	return (i.distance<j.distance);	//Does sorting based on the Hessian distance shown by i.distance
}


int depth_height_calc(Mat leftImg, Mat rightImg) 
{

	vector< DMatch > m1;	//m1 is a vector of type DMatch which contains matches	
	
	clock_t s1,s2,s3,s4,start, end,end2,end3,end4,end5,end6,end7,end8; //Clock variables to measure time
	
	int number_of_matches = 10;	//number of matches to be considered finally. Explained in detail ahead

	start = clock();	//Start clock
	
/**************RECTIFY IMAGES***************/

/* remap is used to rectify captured images using the extrinsic parameters of the camera. The left rectified image is stored in img_left_remap and right rectified image is stored in img_right_remap. */

	remap(leftImg, img_left_remap, rmap[0][0], rmap[0][1], CV_INTER_LINEAR);
	remap(rightImg, img_right_remap, rmap[1][0], rmap[1][1], CV_INTER_LINEAR);

	end2=clock()-(start);	//time taken to rectify images is stored in end2.
	printf("Rectification Time Taken=%f msec\n",(((double)end2/CLOCKS_PER_SEC)*1000.00));


/**************FAST FEATURE DETECTION***************/	
	
	detector.detect(img_left_remap, keypoints_1);	//keypoints are stored in keypoints_1 and keypoints_2.
	detector.detect(img_right_remap, keypoints_2);
	
	end3=clock()-(end2+start);	//time taken for Fast feature extraction
	printf("Detector Time Taken=%f msec\n",(((double)end3/CLOCKS_PER_SEC)*1000.00));
	

/**************DESCRIPTOR EXTRACTION FROM FEATURE POINTS***************/
	
	ex.compute(img_left_remap, keypoints_1, descriptors_1);
	ex.compute(img_right_remap, keypoints_2, descriptors_2);
	
	end4=clock()-(end3+end2+start);	//time taken for descriptor extraction
	printf("Extraction Time Taken=%f msec\n",(((double)end4/CLOCKS_PER_SEC)*1000.00));


/***************BRUTE FORCE HAMMING FEATURE MATCHING****************/
		
	//BruteForceMatcher<Hamming> matcher;
	BFMatcher matcher(NORM_HAMMING2, false);
	vector< DMatch > matches;	//Creating a vector of DMatch to hold the matches
	matcher.match( descriptors_1, descriptors_2, matches );	//this statement does the matching

	
	end5=clock()-(end4+end3+end2+start);	//time taken for flann based matching
	printf("Good Match extraction Time Taken=%f msec\n",(((double)end5/CLOCKS_PER_SEC)*1000.00));


/************SORTING OF MATCHES ACCORDING TO HESSIAN DISTANCES************/

/* In this part we refine the matches obtained in the above section by sorting them according to distances and the taking a predefined number of matches having the least Hessian Distance. This reduces the number of matches significantly thus reducing the time required*/

	sort(matches.begin(),matches.end(),myobject);	//using quicksort from the <alogrithm> header file
	
	
	if(number_of_matches > matches.size())	//Logic to avoid segmentation faults.
		{
			number_of_matches = matches.size();
		}
	
	vector< DMatch > sort_matches;	//good_matches is a refined vector containing only useful matches
	
	for(int i = 0; i < number_of_matches ; i++)
	{
		sort_matches.push_back(matches[i]);
	}
	
	end6=clock()-(end5+end4+end3+end2+start);	//time taken to get the good matches
	printf("Sorting Time Taken=%f msec\n",(((double)end6/CLOCKS_PER_SEC)*1000.00));


/************DISCARD BAD MATCHES*******************/
	
	std::vector< DMatch > good_matches;
	for(int i = 0; i < number_of_matches ; i++)
	{
		if(abs(keypoints_1[sort_matches[i].queryIdx].pt.y-keypoints_2[sort_matches[i].trainIdx].pt.y)<20)
		{
			good_matches.push_back(sort_matches[i]);
		}
	}


/************DEPTH CALCULATION********************/
	double depth[good_matches.size()];	//depth is a array of size equal to number of matches in good_matches. It contains depths of all these matches.
	Mat depthimg=img_left_remap.clone();	//image containing depths of matches in good_matches
	s1=clock();	
	for(i=0;i<good_matches.size();i++)
	{
		disparity_pixel =  abs(keypoints_1[good_matches[i].queryIdx ].pt.x-keypoints_2[ good_matches[i].trainIdx ].pt.x);	//disparity_pixel contains the difference between x coordinates of good_matches points in both images img_left_remap and img_right_remap.
		distance_mm = baseline_mm * focal_length_pixel*0.44*1.46/(2*1000*disparity_pixel);	//The formula is applied to get depths. The extra numerals in the formula appear because the formula was tempered with to scale up or down the results to get exact depths.
		depth[i]=distance_mm;	//distance of ith match is stored in depth[i]
			
		sprintf(text,"%lf",distance_mm);
		putText(depthimg,text,Point2f(keypoints_1[good_matches[i].queryIdx].pt.x,keypoints_1[good_matches[i].queryIdx].pt.y),FONT_HERSHEY_COMPLEX_SMALL,0.5,cvScalar(0,0,0),1,CV_AA);
		circle(depthimg, Point2f(keypoints_1[good_matches[i].queryIdx].pt.x, keypoints_1[good_matches[i].queryIdx].pt.y), 10, Scalar(0,0,255), 1, 8, 0);
	}
	
	
	end7=clock()-s1;	//time taken to get depths of all the matches and writing them onto an image
	printf("Depth Time Taken=%f msec\n",(((double)end7/CLOCKS_PER_SEC)*1000.00));
	//imshow("Depth of obstacles",depthimg);


/**************HEIGHT CALCULATION******************/

	Mat hedisp=img_left_remap.clone();	//calculated heights are shown in hedisp at the end of this segment	
	double he[good_matches.size()];	//he is a array of size of good_matches and contains the heights of all the matches
	int ch=5;	//camera height
	s2=clock();
	for(i=0; i<good_matches.size();i++)
    	{
	        height = getHeight(h,ch, Point2f(keypoints_1[good_matches[i].queryIdx ].pt.x,keypoints_1[good_matches[i].queryIdx].pt.y), Point2f(keypoints_2[good_matches[i].trainIdx ].pt.x,keypoints_2[good_matches[i].trainIdx].pt.y));	//Height of all ith match of good_matches is calculated by passing it as an argument to getHeight along with the homography matrix and camera height
			he[i]=height;	//height of ith match stored in he[i]
	        string s;
	        ostringstream convert;
	        convert.precision(3);
	        convert << height;
	        s = convert.str();
	        putText(hedisp, s,  Point2f(keypoints_1[good_matches[i].queryIdx ].pt.x,keypoints_1[good_matches[i].queryIdx].pt.y), 	FONT_HERSHEY_COMPLEX, 0.5, (0, 0, 255));
	    }

	end8=clock()-s2;	//time taken to estimate heights of all matches in good_matches and display them on the image hedisp
	printf("Height Time Taken=%f msec\n",(((double)end8/CLOCKS_PER_SEC)*1000.00));
	//imshow("heights of obstacles",hedisp); 

//



/************ALGORITHM TO AVOID OBSTALCES BASED ON INFORMATION OBTAINED FROM HEIGHTS AND DEPTHS**************/

	obcount=0;	//number of obstacles initialized to zero
	for(i=0	;i<good_matches.size();i++)
	{
	//	printf("%lf -----------> %lf\n",depth[i],he[i]);	//print the depths and heights just to keep a check
		if(he[i]>=3)	//heights of obstacles if greater than 2 are considered. If heights < 3 then they are either glares or textures or may be even bottom points of obstacles - PROBLEM TO BE CONCERNED ABOUT!!!
		{
			if(depth[i]<1)	//if distance is less than 1 meter than it is to be treated as an obstacle
			{
				obcount++;	//Number of obstacles are incremented
			}
		}
	}

	end=clock()-start;	//time taken for whole code to run
	printf("Time Taken=%f msec\n",(((double)end/CLOCKS_PER_SEC)*1000.00));

	if(obcount>2)	//Obcount is kept because sometimes due to poor matching results, some faraway objects might be considered to be obstacles. But more than 1 of such matches is very less probable.
	{
		printf("OBSTACLES AHEAD\n");
		return 1;	
	}




return 0;


}


/***********getHeight function*****************/

//getHeight function is used to get heights of different points in the image. The parameters passed are 1. The homography matrix of ground plane. 2. The camera height. 3. Point in img_left_remap for good_matches[i]. 4. Point in img_right_remap for good_matches[i]

//Algorithm: We have the ground plane homography which converts all points on ground in img_left_remap to the respective points in img_right_remap. But as it is for ground plane, if this matrix is applied to any point not on this plane, we wont get the point in img_right_remap as a result. 

//For every point passed as an argument, homography matrix of all planes parallel to the ground is computed at an interval of 1cm. The point passed as an argument is multipled with all the matrices on by one and the resultant is stored in (ex,ey). This is the expected matching point in img_right_remap of the point in img_left_remap.

//After getting all these (ex,ey), the one closest to the actual point in img_right_remap is selected. The particular h for which we got the (ex,ey) is the height in cm for that point.

//The formula to calculated homography matrix of a plane at a distance d from the camera, given the homography matrices of 2 planes at distances d1 and d2 from the camera being h1 and h2. The formula can be checked out at this link.http://www.google.co.in/url?sa=t&rct=j&q=&esrc=s&source=web&cd=9&ved=0CIIBEBYwCA&url=http%3A%2F%2Fsyedurrahman.tripod.com%2Fobstacle_detection.pdf&ei=zIN-UfDFK4PkrAeSz4H4CA&usg=AFQjCNHJ_DutJgjsSjcEp6uCz3PCK0TA8A&sig2=qkTwGn35BPwQdCeIPYFRhQ&bvm=bv.45645796,d.bmk&cad=rja

int getHeight(Mat H, int ch, Point i, Point f)
{
	Mat newH;	//newH will contain the homography matrix of plane at a height 'h' from the ground and parallel to it.
	int h=0, height=0, diff=0;	//h is a varaible height, height is returned as variable height
	int mindiff = 1000;
	int maxh=100;	//maxh is the maximum height till which detection is done in centimeters
	int test;    
	double ex, ey, p, q, r, t, d, s;	//(ex,ey) is the expected point in img_right_remap
    while(h < maxh)	// The matrices will be computed for all planes from 0cm to 100cm. Obstacles higher than that have been avoided here.
    {
        H.copyTo(newH);		//newH=H
	//Mat H_inf = Mat::eye(3,3, CV_64F)*(float(h-ch)/float(h));
        Mat H_inf = Mat::eye(3,3, CV_64F)*(float(h)/float(h-ch));
        /*
        for(int k=0; k<newH.rows; k++)
        {
            for(int j=0; j<newH.cols; j++)
            {
                printf("%f ", H.at<double>(k, j));
            }
            printf("\n");
        }
        */
	    
	newH = H.mul(float(ch)/float(ch-h));
    //    Mat H_inf = Mat::eye(3,3, CV_64F)*(float(h)/float(h-ch));
//	newH = H.mul(float(ch)/float(h-ch));
        Mat A = Mat(3,1, CV_64F);	//A is a 3x1 matrix which contains [x y 1] where x and y are the X and Y coordinates of img_left_remap point
        Mat B = Mat(3,1, CV_64F);	// B will be calculated by multiplying A with newH
        A.at<double>(0,0) = i.x;
        A.at<double>(1,0) = i.y;
        A.at<double>(2,0) = 1;
        
        newH = newH + H_inf;
        
        
        gemm(newH, A, 1.0, Mat(), 0, B);

        ex = B.at<double>(0, 0);	//B is [ex ey 1]
        ey = B.at<double>(1, 0);
        
	
	    p = sqrt((i.x-ex)*(i.x-ex)+(i.y-ey)*(i.y-ey));		// All this calculation is done using the paper data. Link is pasted at top of this function
        r = sqrt((f.x-ex)*(f.x-ex)+(f.y-ey)*(f.y-ey));
        q = sqrt((i.x-f.x)*(i.x-f.x)+(i.y-f.y)*(i.y-f.y));
        t = acos((p*p+q*q-r*r)/(2*p*q))*180/CV_PI;
   	    s = 1 - ((p*p+r*r-q*q)/(2*p*r))*((p*p+r*r-q*q)/(2*p*r));
        d = r*100/p + t;
    
        if (d < mindiff)	// Height at which distance is less is considered as height of the obstacle
        {
            height = h;
            mindiff = d;
	    test=r;
        }
        h++;
    }

return (height);	//returns height to main function
}


