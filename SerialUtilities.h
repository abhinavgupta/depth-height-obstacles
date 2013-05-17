#ifndef SERIALUTILITIES_H
#define SERIALUTILITIES_H

/*Serial functions for initializing and writing*/

int serialport_init(const char* serialport, int baud);
int serialport_write(int fd, const char* str);

#endif
