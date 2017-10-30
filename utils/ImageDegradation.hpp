//
//  ICAOphotoquality
//
//  Copyright © 2017 Marcelo Cobias. All rights reserved.
//
#ifndef ImageDegradation_HPP
#define ImageDegradation_HPP

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class ImageDegradation
{
public:
    static Mat gaussianNoise(Mat image);
    static Mat saltAndPepper(Mat image);
    static Mat bluring(Mat image);
    static Mat smooth(Mat image);
    static Mat basicLinearTransform(Mat image, double alpha = 0, int beta = 0);
    static Mat gamma(Mat image, double gamma = 0);
};
#endif
