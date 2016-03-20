#pragma once

#include <sys/socket.h>

class Receiver {
public:
    Receiver();
    ~Receiver();

private:
    int sockfd;
    int newSockFd;
    const int portno = 2512;
    const int reusePort = 1;
    socklen_t clilen;

    char buffer[256];

    struct sockaddr_in serv_addr, cli_addr;

    void listenPort();
};