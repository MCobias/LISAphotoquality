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

// Brightness(Alpha gain) and contrast(Beta bias)
Mat ImageDegradation::basicLinearTransform(Mat image, double alpha, int beta)
{
    Mat res, imgCorrected;
    image.convertTo(res, -1, alpha, beta);
    hconcat(image, res, imgCorrected);
    return imgCorrected;
}

// Gamma
Mat ImageDegradation::gamma(Mat image, double gamma)
{
    CV_Assert(gamma >= 0);
    Mat imgGammaCorrected;
    Mat lookUpTable(1, 256, CV_8U);
    uchar* p = lookUpTable.ptr();
    for( int i = 0; i < 256; ++i)
        p[i] = saturate_cast<uchar>(pow(i / 255.0, gamma) * 255.0);

    Mat res = image.clone();
    LUT(image, lookUpTable, res);
    hconcat(image, res, imgGammaCorrected);
    return imgGammaCorrected;
}
