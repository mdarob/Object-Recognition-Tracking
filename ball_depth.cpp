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

//g++ -o ball_depth ball_depth.cpp `pkg-config --cflags --libs opencv`

using namespace std;
using namespace cv;

//TRACK A YELLOW BALL - OBJECT DETECTION METHOD USING COLOR SEPERATION OPEN CV 3.1.0   //author - Rachit Gulati

vector<Vec3f> v3fCircles; 
Mat img;
int threshval = 100;

//channel 2 works better I think.

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

    //imshow( "Connected Components", dst );
}

static void help()
{    cout << "\n This program demonstrates connected components and use of the trackbar\n"  "Usage: \n"   "  ./connected_components <image(../data/      		stuff.jpg as default)>\n"  "The image is converted to grayscale and displayed, another image has a trackbar\n"  "that controls thresholding 	and thereby 		the extracted contours which are drawn in color\n";}

const char* keys ={   "{help h||}{@image|../data/stuff.jpg|image for converting to a grayscale}"};

int main() {

	VideoCapture capWebcam(0);		// declare a VideoCapture object to associate webcam, 0 means use 1st (default) webcam
	if (capWebcam.isOpened() == false)	 //  To check if object was associated to webcam successfully
	{cout << "error: Webcam connect unsuccessful\n";	return(0);    }

	VideoCapture leftCam(1);	  //lets say 0 is left, 1 is right
	if (leftCam.isOpened() == false){cout << "error: Webcam connect unsuccessful\n";	return(0);    }

	Mat imgOriginal, left;		// Input image
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

		if (!blnFrameReadSuccessfully || imgOriginal.empty()) {cout << "error: frame can't read \n";	break;}
		
		Mat  frame; //frame is the right camera, id 0
		capWebcam.read(frame);
		if (frame.empty()){cout << "No frame to read" << endl; break;}		//cout << "Printing Mat frame : "  << frame << endl;	
		cout << "Width  " << frame.size().width <<  " Height :  " << frame.size().height << endl;	

		leftCam.read(left);
		if (left.empty()){cout << "No frame to read" << endl;  break;}		
				
		
			    //Depth Map
			    Size imagesize = left.size();
			    Mat disparity_left= Mat(imagesize.height,imagesize.width,CV_16S); 	//height and width of the image
			    Mat disparity_right=Mat(imagesize.height,imagesize.width,CV_16S);
			    Mat g1,g2,disp,disp8;
			    cvtColor(left,g1, COLOR_BGR2GRAY);  		 // convert color to gray
			    cvtColor(frame,g2, COLOR_BGR2GRAY);   //frame is the right camera for now
				
			//MY IMPL
			    Ptr<cv::StereoBM> sbm =  StereoBM::create(0,11);   // numDisparities, blocksize
			    sbm->setDisp12MaxDiff(1);
			    sbm->setSpeckleRange(8);
			    sbm->setSpeckleWindowSize(9);
			    sbm->setUniquenessRatio(0);
			    sbm->setTextureThreshold(507);
			    sbm->setMinDisparity(-39);
			    sbm->setPreFilterCap(61);
			    sbm->setPreFilterSize(5);
			    sbm->compute(g1,g2,disparity_left);    //computes the disparty in 3d
			    normalize(disparity_left, disp8, 0, 255, NORM_MINMAX, CV_8UC3); //8U (8-bit unsigned integer) vs 8UC1 (8-bit unsigned channel 1), 
			    //cout << "disp8 [0][0]" << disp8.rowRange(0,1) << endl;  //row 0 - 2
			
		//*********************************HSV*********************************************************************************
		cvtColor(imgOriginal, hsvImg, COLOR_BGR2HSV);						// Convert Original Image to HSV Thresh Image
		inRange(hsvImg, Scalar(lowH, lowS, lowV), Scalar(highH, highS, highV), threshImg);
		GaussianBlur(threshImg, threshImg, Size(3, 3), 0);			//Blur Effect
		dilate(threshImg, threshImg, 0);		// Dilate Filter Effect
		erode(threshImg, threshImg, 0);		// Erode Filter Effect

		// fill circles vector with all circles in processed image
		HoughCircles(threshImg, v3fCircles, HOUGH_GRADIENT, 2, threshImg.rows / 4, 100, 50, 10, 800);  // algorithm for detecting circles		
		
		//Draw a circle on the Depth Map	
		for (int j = 0; j< v3fCircles.size(); j++)
		{
			circle(disp8, Point((int)v3fCircles[j][0], (int)v3fCircles[j][1]), 3, Scalar(0, 255, 0), FILLED	);	//inner circle, /frame is the right camera, id 0
			circle(disp8, Point((int)v3fCircles[j][0], (int)v3fCircles[j][1]), (int)v3fCircles[j][2], Scalar(244, 66, 66), 5); //outer circle
		}

		for (int i = 0; i < v3fCircles.size(); i++) 
		{	// for each circle
			cout << "Ball position X for HSV = " << v3fCircles[i][0]  << ",\tY = " << v3fCircles[i][1]<< ",\tRadius = " << v3fCircles[i][2] << "\n";
			circle(imgOriginal,Point((int)v3fCircles[i][0], (int)v3fCircles[i][1]),3,Scalar(0,255,0),FILLED); // draw small green circle at center of object detected
			circle(imgOriginal,Point((int)v3fCircles[i][0], (int)v3fCircles[i][1]),	(int)v3fCircles[i][2],Scalar(0,0,255),3);

			if (v3fCircles[i][0] > 300)
			{
				cout << "Circle Pixel is bigge than 300" << endl;
			}
			cout <<  "The number of Circles detected  : " <<  v3fCircles.size() << endl;

			
			
			//disp8  = Mat::zeros(disp8.size(), disp8.type()   );  //This make the disp8 to be zeros,  lets just put the 0-2 index
			//for (int k = 0;  k < 2;  k++)	{	disp8[k][0] = v3fCircles[i][0];cout << disp8[k][0] << endl;	}
			//cout << "Disp8 values :  " << disp8 << endl;			
			//So make alll the Mat disp8 to 0 except V3fCircles [i][0]-[2]!!!!!!!!!!!!!!!!!! Have fun 
			// make zerso of Mat ad then dump only the V3fCircles [i][0]-[2]...
			/*Mat img;
			img  = Mat::zeros(img.size(), img.type()   );
			cout << "All Zeros : " << img <<  endl;		//so now img have all zeros just put the 0-2 index values			
			Mat index_adder = v3fCircles[i][0]  + img; 
			cout << "v3fC values of first Index" << index_adder << endl;*/
			
		}
	
		//const int nSteps = disp8.size();   // thats how many steps
		/*const int nSteps = 640;
		const int nReal = 480;
		const int N = nSteps * nReal;
		double H[N] = {0.0};
		for (int j = 0; j< N; j++)	{ cout << H[j] << endl;// so now make that disp8, make all disp8 to 0	}*/
		

			Mat frame2;
			frame2 = frame.clone();      				//copy from the camera	
			//imwrite("screenshot.png", frame2);	        // write it to screenshot.png in this directory
			//Mat frame3 = imread ("screenshot.png"); //read from the pic/screenshot

			//RGB
			Mat outPutRed;
			inRange(frame2, Scalar(lowR, lowG, lowB), Scalar(highR, highG, highB), outPutRed);
			GaussianBlur(outPutRed, outPutRed, Size(3, 3), 0);
			dilate(outPutRed, outPutRed, 0);
			erode(outPutRed, outPutRed, 0);

			HoughCircles(outPutRed, v3fCircles, HOUGH_GRADIENT, 2, outPutRed.rows / 4, 100, 50, 10, 100); // Algo for circles

			// Connected Componenets
			//img = imread("screenshot.png", 0);
			//if(img.empty()) {cout << "Could not read input image file: " <<  endl;return -1;    }

			for (int i = 0; i< v3fCircles.size(); i++)
			{
				//cout << "Ball Pos X = " << v3fCircles[i][0] << ",\tY = " << v3fCircles[i][1] << ",\tRadians = " << v3fCircles[i][2] << endl;
				circle(frame, Point((int)v3fCircles[i][0], (int)v3fCircles[i][1]), 3, Scalar(0, 255, 0), FILLED	);	//inner circle, /frame is the right camera, id 0
				circle(frame, Point((int)v3fCircles[i][0], (int)v3fCircles[i][1]), (int)v3fCircles[i][2], Scalar(0, 255, 0), 3); //outer circle
			}

		// declare windows
		//namedWindow("imgOriginal", WINDOW_AUTOSIZE);
		//namedWindow("threshImg", WINDOW_AUTOSIZE);
		//imshow("threshImg", threshImg);
		//namedWindow( "Image", 1 );		
		//imshow( "Image", img );	
		//namedWindow( "Connected Components", 1 );
			

		// Create trackbars in "threshImg" window to adjust according to object and environment.
		/*createTrackbar("LowH", "threshImg", &lowH, 179);	//Hue (0 - 179)
		createTrackbar("HighH", "threshImg", &highH, 179);
		createTrackbar("LowS", "threshImg", &lowS, 255);	//Saturation (0 - 255)
		createTrackbar("HighS", "threshImg", &highS, 255);
		createTrackbar("LowV", "threshImg", &lowV, 255);	//Value (0 - 255)
		createTrackbar("HighV", "threshImg", &highV, 255);
		createTrackbar( "Threshold", "Connected Components", &threshval, 255, on_trackbar ); // for conncted coponents
		on_trackbar(threshval, 0);		*/

		//imshow("imgOriginal", imgOriginal);				
                //imshow("screenshot", frame3);

		//RGB stuff
		/*createTrackbar("lowR", "outPutRed", &lowR, 255);
		createTrackbar("highR", "outPutRed", &highR, 255);
		createTrackbar("lowG", "outPutRed", &lowG, 255);
		createTrackbar("highG", "outPutRed", &highG, 255);
		createTrackbar("lowB", "outPutRed", &lowB, 255);
		createTrackbar("highB", "outPutRed", &highB, 255);*/
		
		namedWindow("Right", WINDOW_AUTOSIZE);
		imshow("Right", frame);
		namedWindow("Left", WINDOW_AUTOSIZE);
		imshow("Left", left);	
		namedWindow("Depth map", WINDOW_AUTOSIZE);
	    	imshow("Depth map", disp8 );
	   	//imshow("Disparity Lft map", disparity_left);   //looks like what we want, but too light grey on the screen

		//imshow("Captured frame", frame2);
		//namedWindow("outPutRed", WINDOW_AUTOSIZE);
		//imshow("Red Output From Camera", outPutRed);		

		charCheckForEscKey = waitKey(1);					// delay and get key press
	}

	return(0);
}
