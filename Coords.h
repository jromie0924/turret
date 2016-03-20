#pragma once

//C++
#include <iostream>
#include <sstream>

using namespace std;

class Coords {
public:
	Coords();
	Coords(unsigned int, unsigned int);

	void setCoords(unsigned int, unsigned int);
	unsigned int getX();
	unsigned int getY();

	~Coords();

private:
	unsigned int x;
	unsigned int y;
};