#include <stdio.h>
#include <iostream>
#include <time.h>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;

int main()
{
	clock_t time_1, time_2, time_3, time_4;
	char k;
	VideoCapture cap(0);
	Mat image_capture;
	printf("Entering the loop \n");
	while(1)
	{
		printf("1\n");
		Mat frame;
		printf("2\n");
		char key = (char )waitKey(0);
		printf("3\n");
		switch(key){
			case 27:
			
				return 0;
			
			case ' ':
				time_1 = clock();
				cap >> frame;
				time_2 = clock() - time_1;
				printf("Time Taken=%f msec\n",(((double)time_2/CLOCKS_PER_SEC)*1000.00));
				imshow("Capture",frame);
				break;
        default:
            break;
            }
		
	
	}
	
	cap.release();	
	
	return 0;
}
