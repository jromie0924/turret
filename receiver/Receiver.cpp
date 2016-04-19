#include <cstdlib>
#include <iostream>

#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

#include "Receiver.h"

using namespace std;

Receiver::Receiver() {
    while(true) {
    	sockfd = socket(AF_INET, SOCK_STREAM, 0);
    	if(sockfd < 0) {
    		cerr << "Error opening socket." << endl;
    		exit(0);
    	}

    	bzero((char *) &serv_addr, sizeof(serv_addr));

    	serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = INADDR_ANY;
        serv_addr.sin_port = htons(portno);
        memset(serv_addr.sin_zero, '\0', sizeof(serv_addr.sin_zero));



        if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reusePort, sizeof(reusePort)) == -1) {
            cerr << "Error reusing port." << endl;
            exit(1);
        }

        if(bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
            cerr << "Error on binding." << endl;
            exit(0);
        }

        listenPort();
    }
}

Receiver::~Receiver() {
	// Do nothing
}

void Receiver::listenPort() {
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);
    newSockFd = accept(sockfd, (struct sockaddr*)&cli_addr, &clilen);
    if(newSockFd < 0) {
        cerr << "Error on accept." << endl;
        exit(0);
    }

    bzero(buffer, 256);

    int retVal = read(newSockFd, buffer, 255);
    if(retVal < 0) {
        cerr << "Error reading from socket." << endl;
    }

    string data(buffer);

    cout << data << endl;
    close(sockfd);
    close(newSockFd);
}

int main() {
    Receiver r;
	return 0;
}