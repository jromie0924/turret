//Local
#include "SerialComm.h"

//Other
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <iostream>
#include <errno.h>
#include <fstream>

using namespace std;

//Initialize serial port
void SerialComm::init() {
	xCoord = 0;
	file = fopen("/dev/ttyACM0", "w");
}

void SerialComm::getData(int x) {
	//There are 640 pixels per column
	//int frac = 60*((double)x / 640.0);
	//xCoord = frac + 60;
	sendData();
}

void SerialComm::sendData() {
	fprintf(file, "%d\n", xCoord);
}