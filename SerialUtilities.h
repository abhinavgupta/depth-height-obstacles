#ifndef SERIALUTILITIES_H
#define SERIALUTILITIES_H

int serialport_init(const char* serialport, int baud);
int serialport_write(int fd, const char* str);

#endif
