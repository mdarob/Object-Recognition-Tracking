#include <string>
#include <iostream>
#include <opencv2/opencv.hpp>
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgcodecs/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/calib3d/calib3d.hpp"

using namespace std;
using namespace cv;

int main()
{
	VideoCapture leftCam(0);	  // 0 is left camera, and 1 is right cam
	if (leftCam.isOpened() == false){cout << "error: Webcam connect unsuccessful\n";	return(0);    }
	VideoCapture rightCam(1);	  
	if (rightCam.isOpened() == false){cout << "error: Webcam connect unsuccessful\n";	return(0);    }
	Mat left, right;	//Mat leftClone, rightClone;

	char charCheckForEscKey = 0;

	while (    charCheckForEscKey != 27 && leftCam.isOpened()  )
	{		
		leftCam.read(left);
		if (left.empty()){cout << "No frame to read" << endl;  break;}		
		rightCam.read(right);
		if (right.empty()){cout << "No frame to read" << endl;  break;}		

		    Size imagesize = left.size();
		    Mat disparity_left= Mat(imagesize.height,imagesize.width,CV_16S); 	
		    Mat disparity_right=Mat(imagesize.height,imagesize.width,CV_16S);
		    Mat g1,g2,disp,disp8;
		    cvtColor(left,g1, COLOR_BGR2GRAY);  		
		    cvtColor(right,g2, COLOR_BGR2GRAY);

		    Ptr<cv::StereoBM> sbm =  StereoBM::create(0,11);  
		    
			
		    sbm->setDisp12MaxDiff(1);
		    sbm->setSpeckleRange(8);
		    sbm->setSpeckleWindowSize(9);
		    sbm->setUniquenessRatio(0);
		    sbm->setTextureThreshold(507);
		    sbm->setMinDisparity(-39);
		    sbm->setPreFilterCap(61);
		    sbm->setPreFilterSize(5);
		    sbm->compute(g1,g2,disparity_left);

		    normalize(disparity_left, disp8, 0, 255, NORM_MINMAX, CV_8UC3); //8U (8-bit unsigned integer) vs 8UC1 (8-bit unsigned channel 1)

		    namedWindow("Right", WINDOW_AUTOSIZE);
		    imshow("Right", right);
		    namedWindow("Left", WINDOW_AUTOSIZE);
		    imshow("Left", left);		   		    
		    namedWindow("Depth map", WINDOW_AUTOSIZE);
		    imshow("Depth map", disp8);

		// Colorize the scene
		double min = 0;
		double max = 255;
		minMaxIdx(disp8, &min, &max);
		Mat adjMap;
		disp8.convertTo(adjMap,CV_8UC1, 255 / (max-min), -min); 
		Mat falseColorsMap;
		applyColorMap(adjMap, falseColorsMap, COLORMAP_AUTUMN);
		imshow("Out", falseColorsMap);
		charCheckForEscKey = waitKey(1);	
	}

    return(0);
}
