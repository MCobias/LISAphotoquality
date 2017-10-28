//
//  imagetester.cpp
//  ICAOphotoquality
//
//  Copyright © 2017 Marcelo Cobias. All rights reserved.
//

#include "ImageDegradation.hpp"

Mat ImageDegradation::gaussianNoise(Mat image)
{
  Mat noise = image.clone();
  randn(noise, 128, 30);
  return noise;
}

Mat ImageDegradation::saltAndPepper(Mat image)
{
  Mat saltPepper = Mat::zeros(image.rows, image.cols,CV_8U);
  randu(saltPepper, 0, 255);
  Mat black = saltPepper < 30;
  Mat white = saltPepper > 225;
  Mat result = image.clone();
  result.setTo(255, white);
  result.setTo(0, black);
  return result;
}

Mat ImageDegradation::bluring(Mat image)
{
  Mat result;
  Mat bluring = image.clone();
  blur(bluring, result, cv::Size(5,5));
  return result;
}

Mat ImageDegradation::smooth(Mat image)
{
  Mat result;
  Mat smoothing = image.clone();
  GaussianBlur(smoothing, result, Size(5 ,5), 2.5);
  return result;
}

//add func set max and min: shadows and Lighting
