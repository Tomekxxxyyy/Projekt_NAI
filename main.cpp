#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <cstdio>

using namespace cv;

int main( int argc, char** argv ) {
	int delay = 30;
	char c;
	namedWindow( "Projekt NAI", CV_WINDOW_AUTOSIZE );
	VideoCapture cap(0);
        Mat frame, frame_gray;
	std::vector<Vec3f> circles;
        while(true) {
            cap >> frame;
            cvtColor( frame, frame_gray, CV_BGR2GRAY );
            GaussianBlur( frame_gray, frame_gray, Size(9, 9), 2, 2 );
            HoughCircles( frame_gray, circles, CV_HOUGH_GRADIENT, 1, frame_gray.rows/8, 100, 50, 0, 0 );
            
            for( size_t i = 0; i < circles.size(); i++ )
            {
                Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
                int radius = cvRound(circles[i][2]);
                // środek koła
                circle( frame, center, 3, Scalar(0,255,0), -1, 8, 0 );
                // obramowanie koła
                circle( frame, center, radius, Scalar(0,0,255), 3, 8, 0 );
            }            
            
            imshow( "Projekt NAI", frame );
            c = (char)cv::waitKey(delay);
            if (c == 27) break;	
        }
	
        return 0;
}