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
	
	initialize();																				//Initializing all Global vairiables
	
	VideoCapture cap_left(0), cap_right(1);														//Initializing Captures
	
	cap_left.set(CV_CAP_PROP_FRAME_WIDTH, 320); 												//Setting both capture resolutions
	cap_left.set(CV_CAP_PROP_FRAME_HEIGHT, 240);
	cap_right.set(CV_CAP_PROP_FRAME_WIDTH, 320);
	cap_right.set(CV_CAP_PROP_FRAME_HEIGHT, 240);
	
	int fd = 0;																					//Initializing File Descriptor variable
	int baudrate = B115200;																		//Defining hardcoded baudrate
	char dat[2];																				//Initializing write array 			
	int rc,n;																					//Writing and read handler
	int output_from_matching=0;																	//Value from depth_height_calc();
				
	fd = serialport_init("/dev/ttyS2", baudrate);												//Opening Serialport
	if(fd==-1) return -1;																		//Error handling for Serial port
	
	clock_t a,e,d,c;																			//Clock variables
	
	while(1)																					//Main loop begin
	{
		Mat left_frame, right_frame;															//Reinstantiation of frame matrices
		strcpy(dat, "00\0");																	//Reinitialize write array at begin of loop
		
		a = clock();																			//Get clock
		cap_right >> right_frame;																//Capture Frame
		e = clock()-a;																			//Get clock
		printf("CAMERA CAPTURE = %f msec\n",(((double)e/CLOCKS_PER_SEC)*1000.00));				//Print time taken to capture frame
		
		a = clock();																			//Same procedure as above for second camera
		cap_left >> left_frame;
		e = clock()-a;
		printf("CAMERA CAPTURE = %f msec\n",(((double)e/CLOCKS_PER_SEC)*1000.00));
		
	
		printf("DEPTH-HEIGHT CODE TRIGGERED\n");												//Entering depth-height code
			
		output_from_matching=0;																	//Reinitialize output variable
		output_from_matching = depth_height_calc(left_frame,right_frame);						//Run depth_height code for captured frames
		printf("VALUE FROM DEPTH-HEIGHT CODE IS %d\n",output_from_matching); 					//Print values
			
		if(output_from_matching==0)																//Condition on outcome of depth-height code
		{	
			dat[0] = '2';
			dat[1] = 58;
		}
																								//Self explanatory
		else
		{
			dat[0] = '1';
			dat[1] = 58;
		}
			
		printf("%s\n",dat);																		//Print result
			
		rc = serialport_write(fd, dat);															//Write result to serial port
		if(rc==-1) return -1;																	//Write error handling
	
		
		char key = (char )waitKey(5);															//WaitKey for escape
		if(key==27) return 0;																	//If escape service out of program
		
		
	
	}
	
	cap_left.release();																			//Releasing both camera frames
	cap_right.release();
	
	close(fd);																					//Close serial port
		
	exit(EXIT_SUCCESS);																			//Exit with success
	
	return 0;
}

