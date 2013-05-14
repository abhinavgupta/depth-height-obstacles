#include <stdio.h>    /* Standard input/output definitions */
#include <stdlib.h> 
#include <stdint.h>   /* Standard types */
#include <string.h>   /* String function definitions */
#include <unistd.h>   /* UNIX standard function definitions */
#include <fcntl.h>    /* File control definitions */
#include <errno.h>    /* Error number definitions */
#include <termios.h>  /* POSIX terminal control definitions */
#include <sys/ioctl.h>
#include "SerialUtilities.h"

int main()
{
	int fd = 0;
	int baudrate = B115200;
	char dat[2],b[1];
	int rc;
	
	fd = serialport_init("/dev/ttyUSB0", baudrate);
	if(fd==-1) return -1;
	
	while(1)
	{
		int n = read(fd, b, 1);
		if(b[0]=='i')
			printf("THIS WILL TRIGGER THE DEPTH CODE\n");

	}
	

}

