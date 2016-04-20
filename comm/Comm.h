#pragma once


#include <sys/socket.h>
#include <string.h>

class Comm {
public:
	Comm();
	~Comm();

	void socketConnect(char*);
private:
	int sockfd;
	const int PORTNO = 2512;
	const std::string TARGET = "10.42.0.34";
	
	struct sockaddr_in serv_addr;
	struct hostent* server;
	
	char buffer[256];
};