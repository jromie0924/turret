//C++
#include <iostream>
#include <sstream>

#include "Coords.h"

Coords::Coords() {
	row = 0;
	col = 0;
}

Coords::Coords(int rowIn, int colIn) {
	row = rowIn;
	col = colIn;
}

Coords::~Coords() {
	// Do nothing
}

void Coords::setCoords(int rowIn, int colIn) {
	row = rowIn;
	col = colIn;
}