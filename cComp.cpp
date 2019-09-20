/*
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include "opencv2/objdetect.hpp"
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>

//This program succeffully opens and shows a video
using namespace std;
using namespace cv;

int main(int argc, const char * argv[]) 
{	Mat img;
	VideoCapture capture("v1.avi");
	if (!capture.isOpened())
	throw "ERROR";
	while (1)
	{	
		capture >> img;				
		if (img.empty())	 
			break;			
		imshow("Badminton Video",img);		
		namedWindow("Badminton Video",1);
		waitKey(20);  
	}    
	    waitKey(0);
	    return 0;
}*/


// Example 14-3. Drawing labeled connected components
/*
#include <algorithm>
#include <iostream>
#include<opencv2/opencv.hpp>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include "opencv2/objdetect.hpp"
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>

using namespace std;
using namespace cv;

int main(int argc, char* argv[]) {

  cv::Mat img, img_edge, labels, centroids, img_color, stats, pic;

  // load image or show help if no image was provided
  if( (argc != 2)  || (img = cv::imread( argv[1], cv::IMREAD_GRAYSCALE )).empty()    )
 {
	cout << "\nERROR: You need 2 parameters, you had " << argc << "\n" << endl;
       cout << "\nExample 14-3: Drawing labeled connected componnents\n"
       << "Call is:\n" <<argv[0] <<" <path/image>\n"
      << "\nExample:\n" << argv[0] << " ../HandIndoorColor.jpg\n" << endl; 
    	return -1;
  }

if (argc = 2)
{
	VideoCapture capture(0);
	if (!capture.isOpened()) { throw "ERROR"; }
	capture >> img;	
	img = img_edge;				
	imshow("Badminton Video",img);		
	namedWindow("Badminton Video",1);
	waitKey(20);  
}

  cv::threshold(img, img_edge, 128, 255, cv::THRESH_BINARY);
  cv::imshow("Image after threshold", img_edge);

  int i, nccomps = cv::connectedComponentsWithStats (img_edge, labels,	stats, centroids  );
  cout << "Total Connected Components Detected: " << nccomps << endl;

  vector<cv::Vec3b> colors(nccomps+1);
  colors[0] = cv::Vec3b(0,0,0); // background pixels remain black.
  for( i = 1; i <= nccomps; i++ ) 
{
	//VideoCapture capture(0);
	//if (!capture.isOpened()) { throw "ERROR"; }
	// capture >> img;					
	//imshow("Badminton Video",img);		
	//namedWindow("Badminton Video",1);
	//waitKey(20);  
	
	    if( stats.at<int>(i-1, cv::CC_STAT_AREA) < 100 )
	    colors[i] = cv::Vec3b(0,0,0); // small regions are painted with black too.
	  }
	  img_color = cv::Mat::zeros(img.size(), CV_8UC3);
	  for( int y = 0; y < img_color.rows; y++ )
	 {
	    for( int x = 0; x < img_color.cols; x++ )
	    {
	      int label = labels.at<int>(y, x);
	      CV_Assert(0 <= label && label <= nccomps);
	      img_color.at<cv::Vec3b>(y, x) = colors[label];
	    }
	}
	  cv::imshow("Labeled map", img_color);
	  cv::waitKey();
	  return 0;
}*/


//Exception thrown
/*
#include <opencv2/core/utility.hpp>
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>

using namespace cv;
using namespace std;

Mat img, labelImage;
int threshval = 100;

int main( int argc, const char** argv )
{	
	while (1)
	{	// I have seperately tested out this video openet, I am trying to open the video and tell me from the video the connected componnets
		// that I wants
		VideoCapture capture(0);
		if (!capture.isOpened())
		throw "ERROR";
	       capture >> img;		
		img = imread ("screenshot.png");
		if(img.empty())
		{cout << "Could not read input image file: " << endl;	return -1;    }

		 int nLabels = connectedComponents(img, labelImage, 8, CV_32S); //ccomp(inputArray, label, int connection, CV_32S);
		 cout << " Connected Components " <<  nLabels <<endl;
		std::vector<Vec3b> colors(nLabels);
		    colors[0] = Vec3b(0, 0, 0);// disregard backgrounds 
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
		    namedWindow( "Image", 1 );
		    imshow( "Image", img );
		    namedWindow( "Connected Components", 1 );
		    waitKey(200000000);  
	}
    
	waitKey(0);
        return 0;
}*/

/*
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int main(int argc, const char * argv[]) {

  
while (1)
{
	
	  // Create an image
	    const int color_white = 255;
	    Mat src = imread("screenshot.png");    // = Mat::zeros(600, 800, CV_8UC1);
	    
	    rectangle(src, Point(100, 100), Point(200, 200), color_white, FILLED);
	    rectangle(src, Point(500, 150), Point(600, 450), color_white, FILLED);
	    rectangle(src, Point(350,250), Point(359,251), color_white, FILLED);
	    rectangle(src, Point(354,246), Point(355,255), color_white, FILLED);
	    circle(src, Point(300, 400), 75, color_white, FILLED);

	    imshow("Original", src);
	if (src.empty())
	{break;}

    // Get connected components and stats
    const int connectivity_8 = 8;
    Mat labels, stats, centroids;
    int nLabels = connectedComponentsWithStats(src, labels, stats, centroids, connectivity_8, CV_32S);
    cout << "Number of connected components = " << nLabels << endl << endl;

    cout << "Show label values:" << endl;
    int component1Pixel = labels.at<int>(150,150);
    cout << "pixel at(150,150) = " << component1Pixel << endl;
    int component2Pixel = labels.at<int>(300,550);
    cout << "pixel at(300,550) = " << component2Pixel << endl << endl;

    // Statistics
    cout << "Show statistics and centroids:" << endl;
    cout << "stats:" << endl << "(left,top,width,height,area)" << endl << stats << endl << endl;
    cout << "centroids:" << endl << "(x, y)" << endl << centroids << endl << endl;

    // Print individual stats for component 1 (component 0 is background)
    cout << "Component 1 stats:" << endl;
    cout << "CC_STAT_LEFT   = " << stats.at<int>(1,CC_STAT_LEFT) << endl;
    cout << "CC_STAT_TOP    = " << stats.at<int>(1,CC_STAT_TOP) << endl;
    cout << "CC_STAT_WIDTH  = " << stats.at<int>(1,CC_STAT_WIDTH) << endl;
    cout << "CC_STAT_HEIGHT = " << stats.at<int>(1,CC_STAT_HEIGHT) << endl;
    cout << "CC_STAT_AREA   = " << stats.at<int>(1,CC_STAT_AREA) << endl;

    // Create image with only component 2
    Mat only2;
    compare(labels, 2, only2, CMP_EQ);
    imshow("Component 2", only2);
    //waitKey(0);
}
	
	waitKey(20);
	return 0;

}*/

#include <opencv2/core/utility.hpp>
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>

using namespace cv;
using namespace std;

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

int main( int argc, const char** argv )
{
	CommandLineParser parser(argc, argv, keys);
	if (parser.has("help")){help(); return 0;	    }
	VideoCapture capWebcam(0);
	while (1)
	{	
		Mat frame;
		capWebcam.read(frame);
		if (frame.empty())	{cout << "No frame to read" << endl; break;}
		imshow("Camera", frame);
		Mat frame2;
		frame2 = frame.clone();      				//copy from the camera	
		imwrite("screenshot.png", frame2);	        // write it to screenshot.png in this directory
		Mat frame3 = imread ("screenshot.png"); //read from the pic/screenshot
		imshow("screenshot", frame3);   		// show it to us
	
		img = imread("screenshot.png", 0);
		if(img.empty()) {cout << "Could not read input image file: " <<  endl;return -1;    }

		namedWindow( "Image", 1 );
		imshow( "Image", img );
		namedWindow( "Connected Components", 1 );
		createTrackbar( "Threshold", "Connected Components", &threshval, 255, on_trackbar );
		on_trackbar(threshval, 0);
		waitKey(1);
	}	
	
    waitKey(0);
    return 0;
}
