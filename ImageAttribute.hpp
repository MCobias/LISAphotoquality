//
//  ICAOphotoquality
//
//  Copyright © 2017 Marcelo Cobias. All rights reserved.
//
#ifndef ImageAttribute_HPP
#define ImageAttribute_HPP

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "utils/Util.hpp"

using namespace std;
using namespace cv;

class ImageAttribute
{
public:
      float getBrightness();
      float getContrast();
      float getBlur();
      ImageAttribute(Mat image);
      ~ImageAttribute();
private:
    float brightness = 0.0;
    float contrast = 0.0;
    float blur = 0.0;
    float calcBrightness(Mat image);
    float calcContrast(Mat image);
    float calcBlur(Mat image);
};
#endif
