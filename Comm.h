/* Communication class. This is the utility that sends the data to the Arduino over Wifi */

#pragma once

#include <sys/socket.h>
#include <string.h>

class Comm {
public:
	Comm();
	~Comm();

	void sendData(const char*);
private:
	int sockfd;
	const int PORTNO = 2512;
	const int BUFLEN = 256;
	const char* TARGET = "10.42.0.34";
};