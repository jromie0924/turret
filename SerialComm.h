#ifndef SERIALCOMM_H
#define SERIALCOMM_H

#include <stdio.h>
#include <iostream>
#include <termios.h>

using namespace std;

class SerialComm {
private:
	FILE *file;
	int xCoord;
	void sendData();
public:
	void init();
	void getData(int);
};


#endif