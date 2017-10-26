//
//  ICAOphotoquality
//
//  Copyright © 2017 Marcelo Cobias. All rights reserved.
//

#ifndef ImageDegradation_HPP
#define ImageDegradation_HPP

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

class ImageDegradation
{
public:
    cv::Mat gaussianNoise(cv::Mat image);
    cv::Mat saltAndPepper(cv::Mat image);
    cv::Mat blur(cv::Mat image);
    cv::Mat smooth(cv::Mat image);
};
#endif
