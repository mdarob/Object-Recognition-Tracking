#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/core/core.hpp>

using namespace std;
using namespace cv;

int main ()
{
	Mat draw;
	draw = Mat::zeros(Size(500,500), CV_8UC1);
	rectangle(draw, Point(10,0), Point(200,100), Scalar(255),2,8,0 );
	imshow("Rectangle", draw);
	waitKey(0);
}
