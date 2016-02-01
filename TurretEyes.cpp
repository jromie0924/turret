//Main Program

//opencv
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
//C++
#include <iostream>
#include <sstream>
#include <unistd.h>

//local
#include "SerialComm.h"
#include "BScan.h"

using namespace cv;
using namespace std;

// Global variables
Mat frame; //normal frame
Mat fgMaskMOG2; //fg mask fg mask generated by MOG2 method
Mat *mask;
Ptr<BackgroundSubtractorMOG2> pMOG2; //MOG2 Background subtractor
int keyboard; //input from keyboard

//SerialComm object pointer
SerialComm *serial;

//BScan pointer
BScan *scanner = NULL;

//pointers will be set to the values of the public x & y values in BScan object
int *horiz, *vert;

// function prototype allows it to be called in the main function
// before it is actually defined
void processFeed(void);

int main(int argc, char** argv)
{
    // store the SerialComm object in the heap
    serial = (SerialComm*)malloc(sizeof(SerialComm));
    serial->init();
    //create windows
    namedWindow("Normal", 1);
    namedWindow("Masked Motion Tracking", 1);

    //create Background Subtractor objects
    pMOG2 = createBackgroundSubtractorMOG2();
    processFeed();

    delete scanner;
    mask = NULL;
    horiz = NULL;
    vert = NULL;
    destroyAllWindows();
    cout << "EXITING\n";
    free(serial);
    return EXIT_SUCCESS;
}
void processFeed(void) {
    VideoCapture capture;
    /** CAMERA NUMBER **/
    capture.open(1);
    if(!capture.isOpened()){
        //error in opening the video input
        cerr << "Unable to open video feed." << endl;
        exit(EXIT_FAILURE);
    }
    //read input data. ESC or 'q' for quitting
    while( (char)keyboard != 'q' && (char)keyboard != 27 ){
        //read the current frame
        if(!capture.read(frame)) {
            cerr << "Unable to read next frame." << endl;
            cerr << "Exiting..." << endl;
            exit(EXIT_FAILURE);
        }

        //update the background model
        pMOG2->apply(frame, fgMaskMOG2);

        //INSTANTIATION OF OBJECT TSCAN
        scanner = new BScan(fgMaskMOG2);

        //pointers to the x & y values in BScan
        horiz = &(scanner->xVal);
        vert = &(scanner->yVal);

        //serial->getData(*horiz);
        serial->getData(*horiz); //TEST

        //scan the current binsry frame output from the apply function call
        mask = scanner->scanIt();
        //displays the x & y values where the gun should (probably) aim
        rectangle(*mask, cv::Point(10, 2), cv::Point(100,20),
            cv::Scalar(255,255,255), -1);
        string recStr = to_string(*horiz) + ", " + to_string(*vert);
        putText(*mask, recStr, cv::Point(15, 15),
            FONT_HERSHEY_SIMPLEX, 0.5 , cv::Scalar(0,0,0));

        //show the current frame and the fg masks
        imshow("Normal", frame);
        imshow("Masked Motion Tracking", *mask);
        if(scanner) {
            delete scanner;
            scanner = NULL;
        }
        //get the input from the keyboard
        keyboard = waitKey( 30 );
    }
    //delete capture object
    capture.release();
}