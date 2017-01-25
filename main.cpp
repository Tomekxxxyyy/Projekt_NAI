#include <opencv/cv.hpp>
#include <opencv/highgui.h>
#include <iostream>

int main( int argc, char** argv ) {
	int delay = 30;
	char c;
	cv::namedWindow( "Projekt NAI", CV_WINDOW_AUTOSIZE );
	cv::VideoCapture cap(0);
	while(true) {
            cv::Mat frame;
            cap >> frame;
            cv::imshow( "Projekt NAI", frame );
            c = (char)cv::waitKey(delay);
        if (c == 27) break;	
	}
	return 0;
}