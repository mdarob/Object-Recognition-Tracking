#include<opencv2/core/core.hpp>
#include <opencv2/core/utility.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<iostream>
#include "opencv2/objdetect.hpp"
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>


using namespace std;
using namespace cv;

//TRACK A YELLOW BALL - OBJECT DETECTION METHOD USING COLOR SEPERATION OPEN CV 3.1.0   //author - Rachit Gulati

vector<Vec3f> v3fCircles; 
Mat img;
int threshval = 100;

static void on_trackbar(int, void*)
{
    Mat bw = threshval < 128 ? (img < threshval) : (img > threshval);
    Mat labelImage(img.size(), CV_32S);
    int nLabels = connectedComponents(bw, labelImage, 4);
    std::vector<Vec3b> colors(nLabels);
    colors[0] = Vec3b(0, 0, 0);//background
    for(int label = 1; label < nLabels; ++label){
        colors[label] = Vec3b( (rand()&255), (rand()&255), (rand()&255) );
    }
    Mat dst(img.size(), CV_8UC3);
    for(int r = 0; r < dst.rows; ++r){
        for(int c = 0; c < dst.cols; ++c){
            int label = labelImage.at<int>(r, c);
            Vec3b &pixel = dst.at<Vec3b>(r, c);
            pixel = colors[label];
         }
     }

    imshow( "Connected Components", dst );
}

static void help()
{    cout << "\n This program demonstrates connected components and use of the trackbar\n"  "Usage: \n"   "  ./connected_components <image(../data/      		stuff.jpg as default)>\n"  "The image is converted to grayscale and displayed, another image has a trackbar\n"  "that controls thresholding 	and thereby 		the extracted contours which are drawn in color\n";}

const char* keys ={   "{help h||}{@image|../data/stuff.jpg|image for converting to a grayscale}"};

int main() {

	VideoCapture capWebcam(0);		// declare a VideoCapture object to associate webcam, 0 means use 1st (default) webcam

	if (capWebcam.isOpened() == false)	 //  To check if object was associated to webcam successfully
	{cout << "error: Webcam connect unsuccessful\n";	return(0);    }

	Mat imgOriginal;		// Input image
	Mat hsvImg;				// HSV Image
	Mat threshImg;			// Thresh Image

	std::vector<cv::Vec3f> v3fCircles;   // 3 element vector of floats, this will be the pass by reference output of HoughCircles(), why 3 float?

	char charCheckForEscKey = 0;

	int lowH = 115;							
	int highH = 179;

	int lowS = 92;							
	int highS = 255;

	int lowV = 110;							
	int highV = 255;

	//RGB, Red green blue
	int lowR = 0;
	int highR = 255;

	int lowG = 0;
	int highG = 44;

	int lowB = 83;
	int highB = 255;

	while (charCheckForEscKey != 27 && capWebcam.isOpened())  // until the Esc is pressed or webcam connection is lost
	{				
		bool blnFrameReadSuccessfully = capWebcam.read(imgOriginal);		// get next frame

		if (!blnFrameReadSuccessfully || imgOriginal.empty()) 
		{				// if frame read unsuccessfully
			cout << "error: frame can't read \n";						// print error message
			break;												// jump out of loop
		}

		cvtColor(imgOriginal, hsvImg, COLOR_BGR2HSV);						// Convert Original Image to HSV Thresh Image

		inRange(hsvImg, Scalar(lowH, lowS, lowV), Scalar(highH, highS, highV), threshImg);
		GaussianBlur(threshImg, threshImg, Size(3, 3), 0);			//Blur Effect
		dilate(threshImg, threshImg, 0);								// Dilate Filter Effect
		erode(threshImg, threshImg, 0);									// Erode Filter Effect

		// fill circles vector with all circles in processed image
		HoughCircles(threshImg, v3fCircles, HOUGH_GRADIENT, 2, threshImg.rows / 4, 100, 50, 10, 800);  // algorithm for detecting circles		

		for (int i = 0; i < v3fCircles.size(); i++) 
		{						// for each circle
			//cout << "Ball position X = " << v3fCircles[i][0]	<< ",\tY = " << v3fCircles[i][1]<< ",\tRadius = " << v3fCircles[i][2] << "\n";					
			// draw small green circle at center of object detected
			circle(imgOriginal,Point((int)v3fCircles[i][0], (int)v3fCircles[i][1]),3,Scalar(0,255,0),FILLED);
																			
			circle(imgOriginal,Point((int)v3fCircles[i][0], (int)v3fCircles[i][1]),	(int)v3fCircles[i][2],Scalar(0,0,255),3);                          															
		}

			Mat frame;
			capWebcam.read(frame);
			if (frame.empty()){cout << "No frame to read" << endl; break;}

			//cout << "Width  of Camera  frame :  " << frame.size().width << endl;
			//cout << "Height of Camera  frame :  " << frame.size().height << endl;		

			Mat frame2;
			frame2 = frame.clone();      				//copy from the camera	
			imwrite("screenshot.png", frame2);	        // write it to screenshot.png in this directory
			Mat frame3 = imread ("screenshot.png"); //read from the pic/screenshot

			//RGB
			Mat outPutRed;
			inRange(frame2, Scalar(lowR, lowG, lowB), Scalar(highR, highG, highB), outPutRed);
			GaussianBlur(outPutRed, outPutRed, Size(3, 3), 0);
			dilate(outPutRed, outPutRed, 0);
			erode(outPutRed, outPutRed, 0);

			HoughCircles(outPutRed, v3fCircles, HOUGH_GRADIENT, 2, outPutRed.rows / 4, 100, 50, 10, 100); // Algo for circles

			// Connected Componenets
			img = imread("screenshot.png", 0);
			if(img.empty()) {cout << "Could not read input image file: " <<  endl;return -1;    }

			for (int i = 0; i< v3fCircles.size(); i++)
			{
			//cout << "Ball Pos X = " << v3fCircles[i][0] << ",\tY = " << v3fCircles[i][1] << ",\tRadians = " << v3fCircles[i][2] << endl;
				circle(    frame, Point((int)v3fCircles[i][0], (int)v3fCircles[i][1]), 3, Scalar(0, 255, 0), FILLED	);
				circle(   frame, Point((int)v3fCircles[i][0], (int)v3fCircles[i][1]), (int)v3fCircles[i][2], Scalar(0, 255, 0), 3);
			}

		// declare windows
		namedWindow("imgOriginal", WINDOW_AUTOSIZE);
		namedWindow("threshImg", WINDOW_AUTOSIZE);
		//namedWindow( "Image", 1 );			
		namedWindow( "Connected Components", 1 );
			

		// Create trackbars in "threshImg" window to adjust according to object and environment.
		createTrackbar("LowH", "threshImg", &lowH, 179);	//Hue (0 - 179)
		createTrackbar("HighH", "threshImg", &highH, 179);
		createTrackbar("LowS", "threshImg", &lowS, 255);	//Saturation (0 - 255)
		createTrackbar("HighS", "threshImg", &highS, 255);
		createTrackbar("LowV", "threshImg", &lowV, 255);	//Value (0 - 255)
		createTrackbar("HighV", "threshImg", &highV, 255);
		createTrackbar( "Threshold", "Connected Components", &threshval, 255, on_trackbar ); // for conncted coponents
		on_trackbar(threshval, 0);		
		imshow("imgOriginal", imgOriginal);					// show windows
		imshow("threshImg", threshImg);
		imshow( "Image", img );
                imshow("screenshot", frame3);

		//RGB stuff
		createTrackbar("lowR", "outPutRed", &lowR, 255);
		createTrackbar("highR", "outPutRed", &highR, 255);
		createTrackbar("lowG", "outPutRed", &lowG, 255);
		createTrackbar("highG", "outPutRed", &highG, 255);
		createTrackbar("lowB", "outPutRed", &lowB, 255);
		createTrackbar("highB", "outPutRed", &highB, 255);

		imshow("Camera", frame);
		//imshow("Captured frame", frame2);
		imshow("Red Output From Camera", outPutRed);

		namedWindow("Camera", WINDOW_AUTOSIZE);
		namedWindow("outPutRed", WINDOW_AUTOSIZE);

		charCheckForEscKey = waitKey(1);					// delay and get key press
	}

	return(0);
}
