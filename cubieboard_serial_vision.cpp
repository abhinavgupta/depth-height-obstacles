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


int main()
{
	VideoCapture cap_left(0);
	VideoCapture cap_right(1);
	
	
	cap1.set(CV_CAP_PROP_FRAME_WIDTH, 320);
	cap1.set(CV_CAP_PROP_FRAME_HEIGHT, 240);
	cap2.set(CV_CAP_PROP_FRAME_WIDTH, 320);
	cap2.set(CV_CAP_PROP_FRAME_HEIGHT, 240);
	
	int fd = 0;
	int baudrate = B115200;
	char dat[2],b[1];
	int rc,n;
	int output_from_matching;
	
	
	
	
	fd = serialport_init("/dev/ttyS2", baudrate);
	if(fd==-1) return -1;
	
	while(1)
	{
		Mat left_frame, right_frame;
		strcpy(dat, "00\0");
		
		cap_left >> left_frame;
		cap_right >> right_frame;
		
		int n = read(fb,b,1);
		
		if(b[0]=='i')
		{
			printf("DEPTH-HEIGHT CODE TRIGGERED\n");
			output_from_matching=0;
			output_from_matching = chirag(left_frame,right_frame);
			printf("VALUE FROM DEPTH-HEIGHT CODE IS %d\n",output_from_matching);
		}
		
	
	}
	
	return 0;
}

