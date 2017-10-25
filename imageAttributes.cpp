//
//  imagetester.cpp
//  ICAOphotoquality
//
//  Copyright © 2017 Marcelo Cobias. All rights reserved.
//

#include "imageAttributes.hpp"

float imageAttributes::brightness(cv::Mat image)
{
    cv::Mat image_gray;
    cv::Mat img_temp = image.clone();
    cvtColor(img_temp, image_gray, CV_BGR2GRAY);
    cv::Mat image_multiply = image_gray.clone();
    cv::multiply(image_multiply, cv::Scalar(0.2126, 0.7152, 0.0722), image_multiply);
    cv::Scalar scalar = cv::sum(image_multiply);
    return (scalar.val[0] + scalar.val[1] + scalar.val[2]) / image_multiply.size().area();
}

float imageAttributes::contrast(cv::Mat image)
{
    float contrast = 0;
    float intensity = 0;
    float brightness = imageAttributes::brightness(image.clone());

    for(int i = 0; i < image.rows; i++ )
    {
        for( int j = 0; j < image.cols; j++ )
        {
            intensity += (image.at<cv::Vec3b>(i,j)[0] * 0.0722) + (image.at<cv::Vec3b>(i,j)[1] * 0.7152) + (image.at<cv::Vec3b>(i,j)[2] * 0.2126); // Blue, green and red
            intensity /= 255.0;
            contrast += pow((brightness - intensity), 2);
        }
    }
    contrast /= image.size().area();
    return contrast;
}

float imageAttributes::blur(cv::Mat image)
{
    short max = -32767;
    cv::Mat image_gray;
    cv::Mat img_temp = image.clone();
    cvtColor(img_temp, image_gray, CV_BGR2GRAY);

    cv::Mat in(image_gray.rows, image_gray.cols, CV_8U);
    cv::Mat out(image_gray.rows, image_gray.cols, CV_16S);
    in = image_gray.clone();

    out = cv::Scalar::all(0);
    cv::Laplacian(in, out, CV_16S, 1);
    for(int i = (int)(out.rows * 0.15); i < (int)(out.rows * 0.85); i++ )
    {
        for (int j = (int)(out.cols * 0.15); j < (int)(out.cols * 0.85); j++ )
        {
            if (out.at<short int>(i, j) > max)
            {
                max = out.at<short int>(i, j);
            }
        }
    }
    return max;
}

// add func calcule nivel noise (Histogram)
