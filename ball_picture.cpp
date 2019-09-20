#include<opencv2/core/core.hpp>
#include <opencv2/core/utility.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<iostream>
#include "opencv2/objdetect.hpp"
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <string>
#include "opencv2/calib3d/calib3d.hpp"

using namespace std;
using namespace cv;

// THIS PROGRAM SHOULD OPEN PICTURE AND TELL ME WHERE THE POSITION OF THE RED BALL
// IS SO THAT I CAN GO INSIDE THE MAT AND SEE THE EXACT VALUES.
//EVERNTUALLY MY GOAL IS TO KNOW WHERE THE MATRIX IS

vector<Vec3f> v3fCircles; 
Mat frame;

int main() {

	std::vector<cv::Vec3f> v3fCircles;   // 3 element vector of floats, this will be the pass by reference output of HoughCircles(), why 3 float?

	//RGB, Red green blue
	int lowR = 0;
	int highR = 255;

	int lowG = 0;
	int highG = 44;

	int lowB = 83;
	int highB = 255;

	VideoCapture capWebcam(0);	
	capWebcam.read(frame);
	
while(1)
{
	cout << "Width  of Camera  frame :  " << frame.size().width << endl;
	cout << "Height of Camera  frame :  " << frame.size().height << endl;		

	imwrite("redball.png", frame);	             // write it to screenshot.png in this directory
	Mat frame3 = imread ("redball.png");     //read from the pic/screenshot
	
	//cvtColor(imgOriginal, hsvImg, COLOR_BGR2HSV);						// Convert Original Image to HSV Thresh Image

	//RGB
	Mat outPutRed;
	inRange(frame3, Scalar(lowR, lowG, lowB), Scalar(highR, highG, highB), outPutRed);
	GaussianBlur(outPutRed, outPutRed, Size(3, 3), 0);
	dilate(outPutRed, outPutRed, 0);
	erode(outPutRed, outPutRed, 0);

	HoughCircles(outPutRed, v3fCircles, HOUGH_GRADIENT, 2, outPutRed.rows / 4, 100, 50, 10, 100); // Algo for circles

	for (int i = 0; i< v3fCircles.size(); i++)
	{
		cout << "Ball Pos X = " << v3fCircles[i][0] << ",\tY = " << v3fCircles[i][1] << ",\tRadians = " << v3fCircles[i][2] << endl;
		circle(    frame3, Point((int)v3fCircles[i][0], (int)v3fCircles[i][1]), 3, Scalar(0, 255, 0), FILLED	);	//inner circle
		circle(   frame3, Point((int)v3fCircles[i][0], (int)v3fCircles[i][1]), (int)v3fCircles[i][2], Scalar(0, 255, 0), 3); //outer circle
		
		cout << "This is the Matrix at the beginig " << v3fCircles[i][0] << endl;
		cout << "This is the Matrix at the end " << v3fCircles[i][3] <<  "  " << v3fCircles[i][4] << endl;
	}

	

	imshow("Red Ball", frame3);
	waitKey(0);
}
	return(0);
}
