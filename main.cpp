#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <cstdio>
/*
link do projektu https://github.com/Tomekxxxyyy/Projekt_NAI
wykorzystałem originalne tutoriale http://docs.opencv.org/2.4/doc/tutorials/tutorials.html      
*/
using namespace cv;
using namespace std;
int main( int argc, char** argv ) {
	int delay = 30;
        int low_h=0, low_s=0, low_v=0;
        int high_h=255, high_s=255, high_v=255;
        int canny_edge = 100, threshold = 50;
	char c;
        
        namedWindow( "Projekt NAI", CV_WINDOW_AUTOSIZE );
        namedWindow( "Control", CV_WINDOW_AUTOSIZE );
                
	VideoCapture cap(0);
        Mat frame, frame_gray, gaussian_blur, imgHSV, frame_threshold;
       	vector<Vec3f> circles;
        vector<vector<Point> > contours;
        
        createTrackbar("Low H","Control", &low_h, 255);
        createTrackbar("High H","Control", &high_h, 255);
        createTrackbar("Low S","Control", &low_s, 255);
        createTrackbar("High S","Control", &high_s, 255);
        createTrackbar("Low V","Control", &low_v, 255);
        createTrackbar("High V","Control", &high_v, 255);
        
        createTrackbar("Canny Edge","Control", &canny_edge, 250);
        createTrackbar("Threshold","Control", &threshold, 150);
        
        
        while(true) {
            cap >> frame;
            // Wykrywanie koła
            cvtColor( frame, frame_gray, CV_BGR2GRAY );
            GaussianBlur( frame_gray, gaussian_blur, Size(9, 9), 2, 2 );
            HoughCircles( gaussian_blur, circles, CV_HOUGH_GRADIENT, 1, gaussian_blur.rows/8, canny_edge, threshold, 0, 0 );
            
            // Wykrywanie koloru
            cvtColor(frame, imgHSV, COLOR_BGR2HSV);
            inRange(imgHSV,Scalar(low_h,low_s,low_v), Scalar(high_h,high_s,high_v),frame_threshold);
            
            erode(frame_threshold, frame_threshold, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
            dilate(frame_threshold, frame_threshold, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
            
            dilate(frame_threshold, frame_threshold, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 
            erode(frame_threshold, frame_threshold, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
            
            findContours(frame_threshold, contours, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
            
            vector<vector<Point> > contours_poly( contours.size() );
            vector<Rect> boundRect( contours.size() );
            
            for( int i = 0; i < contours.size(); i++ ){ 
                approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
                boundRect[i] = boundingRect( Mat(contours_poly[i]) );
            }
            
            for( int i = 0; i< contours.size(); i++ ){
                Scalar color = Scalar(0,0,255);
                rectangle( frame, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0 );
            }
            
            //Pętla rysuje koła
                for( size_t i = 0; i < circles.size(); i++ ){
                    Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
                    int radius = cvRound(circles[i][2]);
                    // środek koła
                    circle( frame, center, 3, Scalar(0,255,255), -1, 8, 0 );
                    // obramowanie koła
                    circle( frame, center, radius, Scalar(0,255,255), 3, 8, 0 );
                }            
                        
            imshow( "Projekt NAI", frame );
            imshow( "Control", frame_threshold);
            c = (char)cv::waitKey(delay);
            if (c == 27) break;	
        }
	
        return 0;
}