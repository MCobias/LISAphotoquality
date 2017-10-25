//
//  ICAOphotoquality
//
//  Copyright © 2017 Marcelo Cobias. All rights reserved.
//

#ifndef imageAttributes_HPP
#define imageAttributes_HPP

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

class imageAttributes
{
public:
    float brightness(cv::Mat image);
    float contrast(cv::Mat image);
    float blur(cv::Mat image);

    imageAttributes(cv::Mat image)
    {
      cout << "Brightness: " << brightness(image) << endl;
      cout << "Contrast: " << contrast(image) << endl;
      cout << "Blur: " << blur(image) << endl;
    }
};
#endif
