OPENCV_INCLUDEPATH = /usr/local/include
OPENCV_LIBPATH = /usr/local/lib

CC = g++
CFLAGS = -std=c++11 `pkg-config --cflags --libs opencv` -I $(OPENCV_INCLUDEPATH) -L $(OPENCV_LIBPATH)
HEADERS = $(wildcard *.h)
SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = MotionDetector.app
# For debugging
#CFLAGS += -g

.PHONY: default all clean

default: $(EXECUTABLE)

all: default

%.o: %.cpp $(HEADERS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(CFLAGS) -Wall -o $@



clean:
	-rm -f *.o
	-rm -f $(EXECUTABLE)

