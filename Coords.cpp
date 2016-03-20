//C++
#include <iostream>
#include <sstream>

Coords::Coords() {
	x = 0;
	y = 0;
}

Coords::Coords(unsigned int xIn, unsigned int yIn) {
	x = xIn;
	y = yIn;
}

~Coords() {
	// Do nothing
}

void Coords::setCoords(unsigned int xIn, unsigned int yIn) {
	x = xIn;
	y = yIn;
}

unsigned int Coords::getX() {
	return x;
}

unsigned int getY() {
	return y;
}

