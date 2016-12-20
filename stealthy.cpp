#include <iostream>
#include <vector>
#include <stdio.h>

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/video/background_segm.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;

int main()
{

	//setup filename--------------------------------------------------------------
	int vidNum = 0; int picNum = 0;
	char filename[1000];  //reserve enough space for the filename string
	sprintf(filename,"Stealthy Video%.2d.mov",vidNum++);
	//----------------------------------------------------------------------------

	int frameRate = 15;
	int changes = 0;  //number of red pixel occurrances

	VideoCapture capture;
	bool update_bg_model = true;

	capture.open(0);
	cv::BackgroundSubtractorMOG2 bg;
	bg.set ("nmixtures", 3);
	std::vector < std::vector < cv::Point > >contours;

	cv::namedWindow ("Stealthy - Live Video");
	cv::namedWindow ("Stealthy - Initial State");

	Mat frame, fgmask, fgimg, backgroundImage, checker;
	capture >> frame;

	VideoWriter* writer = NULL;

	writer = new VideoWriter(filename,
							CV_FOURCC('j','p','e','g'),
					    frameRate,Size(frame.cols,frame.rows));

  for(;;) {
  	capture >> frame;

	  bg.operator()(frame, fgimg);
		bg.getBackgroundImage (backgroundImage);
		cv::erode (fgimg, fgimg, cv::Mat ());
		cv::dilate (fgimg, fgimg, cv::Mat ());

		cv::findContours (fgimg, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
		cv::drawContours (frame, contours, -1, cv::Scalar (0, 0, 255), 2);

		if (frame.empty())
			continue;

		cv::imshow ("Stealthy - Live Video", frame);
		cv::imshow ("Stealthy - Initial State", backgroundImage);

		writer->write(frame);

		/*
		Scan every pixel in the frame to check if there are more than 400,000
		red pixels.  If there are, then there's a significant change in image
		which means someone is probably walking around.
		*/
		for (int row = 0; row < frame.rows; row++) {
			for (int col = 0; col < frame.cols; col++) {
				if (frame.at<cv::Vec3b>(row, col) == Vec3b(0,0,255)) {
					changes++;
				}
				else if (changes >= 400000) {
					//take a picture of them!
					sprintf(filename,"Stealthy Photo%.4d.jpg",picNum++);
          imwrite(filename,frame);
          cout << "Saved " << filename << endl;
					changes = 0;
				}
			}
		}

		char c = (char)waitKey(5);
		if(c == 27){
			break;
		}
	}

	return 0;
}
