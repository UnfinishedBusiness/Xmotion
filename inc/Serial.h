#ifndef SERIAL_
#define SERIAL_
#include <application.h>

int Serial_Init();
void Serial_Close();
void Serial_Parse();
void Serial_Read();
void Serial_WriteByte(uint8_t);
void Serial_WriteString(std::string);

long map_range(long, long, long, long, long);
int set_interface_attribs (int, int, int);
void set_blocking (int, int);
#endif
