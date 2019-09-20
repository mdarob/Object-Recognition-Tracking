/*#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
    Mat image = imread("green.png");
	    for( int i = 0 ; i < image.rows ; i++)
	    {
			for( int j = 0 ; j < image.cols ; j++ )
			{
				    if(image.type() == CV_8UC1)
				    {
					image.at<uchar>(i,j) = 255;
				    }
				    else if(image.type() == CV_8UC3)
				    {
					cout << (int) image.at<Vec3b>(i,j)[0] << " " << (int) image.at<Vec3b>(i,j)[1]* 0 << " " <<(int) image.at<Vec3b>(i,j)[2] * 0 << endl;

				        //image.at<Vec3b>(i,j)[0] = 255;
					//image.at<Vec3b>(i,j)[1] = 255;
					//image.at<Vec3b>(i,j)[2] = 255;

					//cout << (int) image.at<Vec3b>(i,j)[0] << " " << (int) image.at<Vec3b>(i,j)[1] << " " << (int) image.at<Vec3b>(i,j)[2] << endl;
				    }
				    else
				    {
					cout << "Unknown image format" << endl;
					return 0;
				    }
			}
	    }

    imshow("Result İmage", image);
    waitKey(0);
}*/

/*
#include <opencv2/opencv.hpp>
#include <stdint.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <opencv2/core/core.hpp>


using namespace std;
using namespace cv;

int main(int argc, char** argv )
{
	Mat original = imread("download.png"); //, CV_LOAD_IMAGE_COLOR);
	Mat mod = imread("download.png"); //,  CV_LOAD_IMAGE_COLOR);

	for (int i = 0; i < mod.rows; i++)
	{
		for (int j = 0; j < mod.cols; j++)
		{
			mod.at<uint8_t> (i,j) = mod.at <uint8_t> (i,j) * 0.5f;
		}
	}
	imshow("Original", original);
	imshow("modified", mod);
	waitKey();
}*/

/*
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int main(int argc, const char * argv[]) {

    // Create an image
    const int color_white = 255; // white at its best
    //Mat src = Mat::zeros(600, 800, CV_8UC1); // 600 by 800 pixel image of zeros 
      Mat src = imread("screenshot.png");

    //rectangle(src, Point(100, 100), Point(200, 200), color_white, FILLED);
    //rectangle(src, Point(500, 150), Point(800, 450), color_white, FILLED);
    //rectangle(src, Point(350,250), Point(359,251), color_white, FILLED);
    //rectangle(src, Point(354,246), Point(355,255), color_white, FILLED);
    //circle(src, Point(300, 400), 75, color_white, FILLED);

    imshow("Original", src);

    cout << "Width  of Camera  frame :  " << src.size().width << endl;
    cout << "Height of Camera  frame :  " << src.size().height << endl;	

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

    waitKey(0);

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
    Mat labelImage(img.size(), CV_32S);  //what is 32 S???
    int nLabels = connectedComponents(bw, labelImage, 8);
    std::vector<Vec3b> colors(nLabels);      //what Vec3b??
    colors[0] = Vec3b(0, 0, 0);//background
    for(int label = 1; label < nLabels; ++label){
        colors[label] = Vec3b( (rand()&255), (rand()&255), (rand()&255) );
    }
    Mat dst(img.size(), CV_8UC3);   //8UC3 ???
    for(int r = 0; r < dst.rows; ++r){
        for(int c = 0; c < dst.cols; ++c){
            int label = labelImage.at<int>(r, c);  //using at method 
            Vec3b &pixel = dst.at<Vec3b>(r, c);
            pixel = colors[label];
         }
     }

    imshow( "Connected Components", dst );
}

static void help()
{
    cout << "\n This program demonstrates connected components and use of the trackbar\n"
             "Usage: \n"
             "  ./connected_components <image(../data/stuff.jpg as default)>\n"
             "The image is converted to grayscale and displayed, another image has a trackbar\n"
             "that controls thresholding and thereby the extracted contours which are drawn in color\n";
}

const char* keys =
{
    "{help h||}{@image|  screenshot.png   |image for converting to a grayscale}"
};

int main( int argc, const char** argv )
{
    CommandLineParser parser(argc, argv, keys);
    if (parser.has("help"))
    {
        help();
        return 0;
    }
    
    string inputImage = parser.get<string>(0);
    img = imread(inputImage.c_str(), 0);
	//VideoCapture capture("v1.avi");
	//capture >> img;
	
    if(img.empty())
    {
        cout << "Could not read input image file: " << endl; // inputImage << endl;
        return -1;
    }

    namedWindow( "Image", 1 );
    imshow( "Image", img );

    namedWindow( "Connected Components", 1 );
    createTrackbar( "Threshold", "Connected Components", &threshval, 255, on_trackbar );
    on_trackbar(threshval, 0);

    waitKey(0);
    return 0;
}

