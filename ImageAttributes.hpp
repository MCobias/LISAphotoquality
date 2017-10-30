//
//  ICAOphotoquality
//
//  Copyright © 2017 Marcelo Cobias. All rights reserved.
//
#ifndef ImageAttributes_HPP
#define ImageAttributes_HPP

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "utils/Utils.hpp"

using namespace std;
using namespace cv;

class ImageAttributes
{
public:
    float brightness(Mat image);
    float contrast(Mat image);
    float blur(Mat image);

    ImageAttributes(cv::Mat image)
    {
      cout << "Brightness: " << brightness(image) << endl;
      cout << "Contrast: " << contrast(image) << endl;
      cout << "Blur: " << blur(image) << endl;
    }
};
#endif
