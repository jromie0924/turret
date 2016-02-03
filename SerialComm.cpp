//Local
#include "SerialComm.h"

//Other
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <exception>

using namespace std;

struct serialException : public exception {
  const char * what() const throw() {
    return "Cannot connect to serial port. Is arduino connected?";
  }
};

bool exists(string& name) {
	ifstream f(name.c_str());
	if(f.good()) {
		f.close();
		return true;
	} else {
		f.close();
		return false;
	}
}

//Initialize serial port
void SerialComm::init() {
	xCoord = 0;
	string serialPort = "/dev/ttyACM0";
	try {
		if(!exists(serialPort)) {
			throw serialException();
		} else {
			file = fopen("/dev/ttyACM0", "w"); //needs to be closed
		}
	} catch (serialException& e) {
		cout << e.what() << endl;
	}
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

SerialComm::~SerialComm() {
	fclose(file);
}