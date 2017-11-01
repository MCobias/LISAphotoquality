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
      float getBrightness();
      float getContrast();
      float getBlur();
      ImageAttributes(Mat image);
      ~ImageAttributes();
private:
    float brightness = 0.0;
    float contrast = 0.0;
    float blur = 0.0;
    float calcBrightness(Mat image);
    float calcContrast(Mat image);
    float calcBlur(Mat image);
};
#endif
