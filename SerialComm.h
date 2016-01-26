#ifndef SERIALCOMM_H
#define SERIALCOMM_H

#include <stdio.h>
#include <iostream>
#include <termios.h>

using namespace std;

class SerialComm {
private:
	struct termios tty;
	struct termios tty_old;
	int xCoord;
	void sendData();
public:
	int init();
	void getData(int);
};


#endif