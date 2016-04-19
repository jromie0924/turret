#pragma once


#include <sys/socket.h>
#include <string.h>

class Comm {
public:
	Comm();
	~Comm();

	void socketConnect(int);
private:
	int sockfd;
	const int PORTNO = 2512;
	const std::string TARGET = "192.168.42.16";
	
	struct sockaddr_in serv_addr;
	struct hostent* server;
	
	char buffer[256];
};