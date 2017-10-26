//
//  imagetester.cpp
//  ICAOphotoquality
//
//  Copyright © 2017 Marcelo Cobias. All rights reserved.
//

#include "Utils.hpp"

bool Utils::existsImage(cv::Mat image)
{
    return (!image.empty());
}

cv::Mat Utils::cutImage(cv::Mat image, cv::Rect form)
{
    cv::Mat imageRoi;
    cv::Mat rect = image.clone();
    if(existsImage(image))
    {
        imageRoi = rect(form);
        return imageRoi;
    }
    return imageRoi;
}
