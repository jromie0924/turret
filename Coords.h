#pragma once

//C++
#include <iostream>
#include <sstream>

using namespace std;

class Coords {
public:
	Coords();
	Coords(int, int);

	void setCoords(int, int);

	int row;
	int col;

	~Coords();

};