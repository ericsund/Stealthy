#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <vector>
#include <stdio.h>
#include <ctime>

using namespace cv;
using namespace std;

int process(VideoCapture& capture) {
    //settings------------------------------------------------------------------
    int frameRate = 15;
    //--------------------------------------------------------------------------

    //get date and time---------------------------------------------------------
    //may use all this for something later...
    time_t rawtime;
    struct tm * timeinfo;
    char buffer[80];
    time (&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(buffer, 80, "%d-%m-%Y %I:%M:%S", timeinfo);
    string timestring(buffer);
    //--------------------------------------------------------------------------

    //setup filename and interface----------------------------------------------
    int vidNum = 0; int picNum = 0;
    char filename[1000];  //reserve enough space for the filename string
    sprintf(filename,"Stealthy Video%.2d.mov",vidNum++);
    namedWindow("Stealthy", CV_WINDOW_KEEPRATIO);
    //--------------------------------------------------------------------------

    //setup frames and write them-----------------------------------------------
    Mat frame;
    capture >> frame;
    VideoWriter* writer = NULL;

    writer = new VideoWriter(filename,
                 CV_FOURCC('j','p','e','g'),
                 frameRate,Size(frame.cols,frame.rows));
    //--------------------------------------------------------------------------

    for (;;) {
        capture >> frame;

        if (frame.empty())
            continue;
        imshow("Stealthy", frame);

        writer->write(frame);
        char key = (char)waitKey(5); //delay N millis, usually long enough to display and capture input
        switch (key) {

          //escape key quits
          case 27:
            if(writer != NULL) {
              delete writer;
            }
          return 0;

          //space saves an image
          case ' ':
            sprintf(filename,"Stealthy Photo%.3d.jpg",picNum++);
            imwrite(filename,frame);
            cout << "Saved " << filename << endl;
            break;

          default:
            break;
        }
    }
    return 0;
}

int main(int ac, char** av) {
    std::string arg = av[1];
    VideoCapture capture(arg); //try to open string, this will attempt to open it as a video file
    if (!capture.isOpened()) //if this fails, try to open as a video camera, through the use of an integer param
        capture.open(atoi(arg.c_str()));
    if (!capture.isOpened()) {
        cerr << "Failed to open a video device or video file!\n" << endl;
        return 1;
    }
    return process(capture);
}
