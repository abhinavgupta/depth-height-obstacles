#include <stdio.h>    /* Standard input/output definitions */
#include <stdlib.h> 
#include <stdint.h>   /* Standard types */
#include <string.h>   /* String function definitions */
#include <unistd.h>   /* UNIX standard function definitions */
#include <fcntl.h>    /* File control definitions */
#include <errno.h>    /* Error number definitions */
#include <termios.h>  /* POSIX terminal control definitions */
#include <sys/ioctl.h>
#include <getopt.h>
#include "SerialUtilities.h"
#include "opencv2/opencv.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <time.h>
#include "DepthHeightCalculation.h"
#include "Global.h"

using namespace cv;
using namespace std;

int main()
{
	
	initialize();
	
	int fd = 0;
	int baudrate = B115200;
	char dat[2],b[1];
	int rc,n;
	int output_from_matching=0;
				
	fd = serialport_init("/dev/ttyUSB0", baudrate);
	if(fd==-1) return -1;
	
	clock_t a,e,d,c;
	
	while(1)
	{
		Mat left_frame, right_frame;
		strcpy(dat, "00\0");
		
		a = clock();
		cap_right >> right_frame;
		e = clock()-a;
		printf("CAMERA CAPTURE = %f msec\n",(((double)e/CLOCKS_PER_SEC)*1000.00));
		
		a = clock();
		cap_left >> left_frame;
		e = clock()-a;
		printf("CAMERA CAPTURE = %f msec\n",(((double)e/CLOCKS_PER_SEC)*1000.00));
		
	
		printf("DEPTH-HEIGHT CODE TRIGGERED\n");
			
		output_from_matching=0;
		output_from_matching = depth_height_calc(left_frame,right_frame);
		printf("VALUE FROM DEPTH-HEIGHT CODE IS %d\n",output_from_matching); 
			
		if(output_from_matching==0)
		{	
			dat[0] = '2';
			dat[1] = 58;
		}
	
		else
		{
			dat[0] = '1';
			dat[1] = 58;
		}
			
		printf("%s\n",dat);
			
		rc = serialport_write(fd, dat);
		if(rc==-1) return -1;
	
		
		char key = (char )waitKey(5);
		if(key==27) return 0;
		
		
	
	}
	
	close(fd);
		
	exit(EXIT_SUCCESS);
	
	return 0;
}

