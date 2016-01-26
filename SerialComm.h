#ifndef SERIALCOMM_H
#define SERIALCOMM_H


using namespace std;

class SerialComm {
private:
	int xCoord;
	void sendData();
public:
	void getData(int x);
};


#endif