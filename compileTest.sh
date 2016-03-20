#build
g++ Estimator.cpp BScan.cpp Coords.cpp -std=c++11 -o test `pkg-config --cflags --libs opencv`
