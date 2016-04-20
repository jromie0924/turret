#include <cstdlib>
#include <string.h>
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <cstring>
#include <string>

#include "Comm.h"

using namespace std;

Comm::Comm() {
	server = gethostbyname(TARGET.c_str());
	if(server == NULL) {
		cerr << "No such host." << endl;
		exit(0);
	}
}

Comm::~Comm() {
	// Do nothing
}
 
void Comm::socketConnect(char* x) {
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0) {
		cerr << "Error openining socket." << endl;
		//exit(0);
	}

	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;

	bcopy((char*)server->h_addr, (char*)&serv_addr.sin_addr.s_addr, server->h_length);

	serv_addr.sin_port = htons(PORTNO);

	const int reusePort = 1;

	while(connect(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0) {
		continue;
	}

	bzero(buffer, 256);

	int retVal = snprintf(buffer, sizeof(buffer), "%s", x);
	cout << buffer << endl;

	retVal = write(sockfd, buffer, strlen(buffer));
	//int convX = htonl(x);
	//retVal = write(sockfd, &convX, sizeof(convX));
	if(retVal < 0) {
		cerr << "Error writing to socket." << endl;
	}

	close(sockfd);
}

int main() {
	cout << "Connecting..." << endl;
	for(int a = 0; ; a++) {
		Comm c;
		string msg = "123456";
		char* cMsg = new char[msg.length() + 1];
		strcpy(cMsg, msg.c_str());
		c.socketConnect(cMsg);
	}

	return 0;
}