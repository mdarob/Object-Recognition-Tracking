/***************************************************************************
*   Copyright (C) 2010 by elsamuko                                        *
*   elsamuko@web.de                                                       *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 3 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
*   This program is distributed in the hope that it will be useful,       *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
*   GNU General Public License for more details.                          *
*                                                                         *
*   You should have received a copy of the GNU General Public License     *
*   along with this program; if not, write to the                         *
*   Free Software Foundation, Inc.,                                       *
*   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
***************************************************************************/

// small wrapper program to create a depthmap out of a stereo pair using opencv
// compile with:
// g++ -O2 -Wall `pkg-config --cflags opencv` `pkg-config --libs opencv` -o opencv-depthmap opencv-depthmap.cpp;

//c
//#include <stdio.h>
//#include <string.h>

//c++
#include <iostream>
#include <string>
#include <utility>

//cv
#include <string>
#include <iostream>
#include <opencv2/opencv.hpp>
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgcodecs/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/calib3d/calib3d.hpp"

#include <cv.h>
#include <legacy.hpp>


bool DEBUG;

using namespace std;

//http://opencv.willowgarage.com/documentation/c/camera_calibration_and_3d_reconstruction.html#findstereocorrespondencegc
pair<CvMat*, CvMat*> calcDepthmap( CvMat* cvImgLeft, CvMat* cvImgRight, int numberOfDisparities, int maxIters ) {

    CvSize size = cvGetSize( cvImgLeft );
    CvMat* disparity_left = cvCreateMat( size.height, size.width, CV_16S );
    CvMat* disparity_right = cvCreateMat( size.height, size.width, CV_16S );
    CvStereoGCState* state = cvCreateStereoGCState( numberOfDisparities, maxIters );
    cvFindStereoCorrespondenceGC( cvImgLeft, cvImgRight, disparity_left, disparity_right, state, 0 );
    cvReleaseStereoGCState( &state );

    CvMat* disparity_left_visual  = cvCreateMat( size.height, size.width, CV_16S );
    CvMat* disparity_right_visual = cvCreateMat( size.height, size.width, CV_16S );
    cvConvertScale( disparity_left,  disparity_left_visual,  -8, 0 );
    cvConvertScale( disparity_right, disparity_right_visual, -8, 0 );

    if( DEBUG ) {
        cvSave( "disparity_left.pgm", disparity_left_visual );
        cvSave( "disparity_right.pgm", disparity_right_visual );
    }

    return pair<CvMat*, CvMat*>( disparity_left_visual, disparity_right_visual );
}

int strToInt( string const& s ) {
    int x;
    std::istringstream in( s );
    in >> x;
    return x;
}

void usage() {
    cout << "Usage: opencv-depthmap [options] leftimage rightimage" << endl;
    cout << "       -h or --help      This message" << endl;
    cout << "       -d or --debug     Debug output" << endl;
    cout << "       -p or --parallax  Default: 16" << endl;
    cout << "       -i or --iters     Default: 2" << endl;
    cout << endl;
}

int main( int argc, char* argv[] ) {

    if( argc > 1 ) {

        // default values
        string leftimage;
        string rightimage;
        string parallax;
        string iters;

        CvMat* cvImgLeft = 0;
        CvMat* cvImgRight = 0;
        bool left = true;

        DEBUG = false;

        // set options
        for( int i = 1; i < argc; i++ ) {
            const char* sw = argv[i];

            if( !strcmp( sw, "-h" ) || !strcmp( argv[i], "--help" ) ) {
                usage();
                return 0;

            } else if( !strcmp( sw, "-d" ) || !strcmp( sw, "--debug" ) ) {
                DEBUG = true;

            } else if( !strcmp( sw, "-p" ) || !strcmp( sw, "--parallax" ) ) {
                if( i + 1 >= argc ) {
                    usage();
                    return 1;
                }

                parallax = argv[++i];

            } else if( !strcmp( sw, "-i" ) || !strcmp( sw, "--iters" ) ) {
                if( i + 1 >= argc ) {
                    usage();
                    return 1;
                }

                iters = argv[++i];

            } else {
                if( left ) {
                    leftimage += sw;
                    left = false;
                } else {
                    rightimage += sw;
                }
            }
        }

        if( leftimage.size() != 0 && rightimage.size() != 0 ) {

            cout << "Left Image:  " << leftimage << endl;
            cout << "Right Image: " << rightimage << endl;

            cvImgLeft  = cvLoadImageM( leftimage.c_str(), 0 );
            cvImgRight = cvLoadImageM( rightimage.c_str(), 0 );

            // cvSaveImage( "left_in.jpg", cvImgLeft );
            // cvSaveImage( "right_in.jpg", cvImgRight );

            if( !cvImgLeft ) {
                cout << "Could not load image file: " << leftimage << endl;
                return 1;
            }

            if( !cvImgRight ) {
                cout << "Could not load image file: " << rightimage << endl;
                return 1;
            }

            //set numberOfDisparities
            int numberOfDisparities = 16;

            if( parallax.size() != 0 ) {
                numberOfDisparities = strToInt( parallax );
            }

            if( numberOfDisparities < 1 ) {
                numberOfDisparities = 1;
            }

            cout << "numberOfDisparities: " << numberOfDisparities << endl;

            //set maxIters
            int maxIters = 2;

            if( iters.size() != 0 ) {
                maxIters = strToInt( iters );
            }

            if( maxIters < 1 ) {
                maxIters = 1;
            }

            cout << "maxIters: " << maxIters << endl;

            //create depthmap
            pair<CvMat*, CvMat*> disparity = calcDepthmap( cvImgLeft, cvImgRight, numberOfDisparities, maxIters );

            //save depthmap
            cvSaveImage( "disparity_left.jpg", disparity.first );
            cvSaveImage( "disparity_right.jpg", disparity.second );

        } else {
            cout << "Error: Not enough arguments:" << endl;
            usage();
        }

    } else {
        usage();
    }

    return 0;
}

//EOF

