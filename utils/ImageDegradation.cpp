//
//  imagetester.cpp
//  ICAOphotoquality
//
//  Copyright © 2017 Marcelo Cobias. All rights reserved.
//

#include "ImageDegradation.hpp"

cv::Mat ImageDegradation::gaussianNoise(cv::Mat image)
{
  cv::Mat noise = image.clone();
  cv::randn(noise, 128, 30);
  return noise;
}

cv::Mat ImageDegradation::saltAndPepper(cv::Mat image)
{
  cv::Mat saltPepper = cv::Mat::zeros(image.rows, image.cols,CV_8U);
  cv::randu(saltPepper, 0, 255);
  cv::Mat black = saltPepper < 30;
  cv::Mat white = saltPepper > 225;
  cv::Mat result = image.clone();
  result.setTo(255, white);
  result.setTo(0, black);
  return result;
}

cv::Mat ImageDegradation::blur(cv::Mat image)
{
  cv::Mat result;
  cv::Mat bluring = image.clone();
  cv::blur(bluring, result, cv::Size(5,5));
  return result;
}

cv::Mat ImageDegradation::smooth(cv::Mat image)
{
  cv::Mat result;
  cv::Mat smoothing = image.clone();
  cv::GaussianBlur(smoothing, result, cv::Size(5,5), 1.5);
  return result;
}

//add func set max and min: shadows and Lighting
