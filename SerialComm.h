#ifndef SERIALCOMM_H
#define SERIALCOMM_H

#include <stdio.h>
#include <iostream>

using namespace std;

class SerialComm {
private:
	FILE *file;
	int xCoord;
	void sendData();
public:
	void init();
	~SerialComm();
	void getData(int);
};


#endif