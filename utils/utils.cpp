//
//  imagetester.cpp
//  ICAOphotoquality
//
//  Copyright © 2017 Marcelo Cobias. All rights reserved.
//

#include "utils.hpp"

bool utils::existsImage(cv::Mat image)
{
    return (!image.empty());
}

cv::Mat utils::cutImage(cv::Mat image, cv::Rect form)
{
    cv::Mat imageroi;
    cv::Mat rect = image.clone();
    if(existsImage(image))
    {
        imageroi= rect(form);
        return imageroi;
    }
    return imageroi;
}
