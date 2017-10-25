//
//  imagetester.cpp
//  ICAOphotoquality
//
//  Copyright © 2017 Marcelo Cobias. All rights reserved.
//

#include "imageDegradation.hpp"

cv::Mat imageDegradation::gaussianNoise(cv::Mat image)
{
  cv::Mat noise = image.clone();
  cv::randn(noise, 128, 30);
  return noise;
}

cv::Mat imageDegradation::saltAndPepper(cv::Mat image)
{
  cv::Mat saltpepper = cv::Mat::zeros(image.rows, image.cols,CV_8U);
  cv::randu(saltpepper, 0, 255);
  cv::Mat black = saltpepper < 30;
  cv::Mat white = saltpepper > 225;
  cv::Mat result = image.clone();
  result.setTo(255, white);
  result.setTo(0, black);
  return result;
}

cv::Mat imageDegradation::blur(cv::Mat image)
{
  cv::Mat result;
  cv::Mat bluring = image.clone();
  cv::blur(bluring, result, cv::Size(5,5));
  return result;
}

cv::Mat imageDegradation::smooth(cv::Mat image)
{
  cv::Mat result;
  cv::Mat smoothing = image.clone();
  cv::GaussianBlur(smoothing, result, cv::Size(5,5), 1.5);
  return result;
}

//add func set max and min: shadows and Lighting
