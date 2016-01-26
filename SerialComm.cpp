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
int SerialComm::init() {
	xCoord = 0;
	memset(&tty, 0, sizeof tty);

	int fd = open("/dev/ttyACM0", O_RDWR | O_NOCTTY | O_NDELAY);

	if(tcgetattr(fd, &tty) != 0) {
		cout << "Error " << errno << " from tcgetattr: " << strerror(errno) << "\n";
	}

	tty_old = tty;

	if(fd == -1) {
		cout << "open_port: Unable to open /dev/ttyACM0\n";
		exit(EXIT_FAILURE);
	}

	cfsetispeed(&tty, B9600);
	cfsetospeed(&tty, B9600);

	tty.c_cflag &= ~PARENB;
	tty.c_cflag &= ~CSTOPB;
	tty.c_cflag &= ~CSIZE;
	tty.c_cflag |= CS8;

	cfmakeraw(&tty);

	tcflush(fd, TCIFLUSH);

	tcsetattr(fd, TCSANOW, &tty);
	return fd;
}

void SerialComm::getData(int x) {
	xCoord = x;
	sendData();
}

void SerialComm::sendData() {
	unsigned char cmd[] = "INIT \r\n";
	int n_written = 0;
	int spot = 0;
	size_t sz = 1;
	do {
		write(tty, &cmd[spot], sz);
	} while(cmd[spot - 1] != '\n' && n_written > 0);
}