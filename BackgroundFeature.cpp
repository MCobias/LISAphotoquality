//
//  ICAOphotoquality
//
//  Copyright © 2017 Marcelo Cobias. All rights reserved.
//
#include "BackgroundFeature.hpp"

/*void identifierface::findLeftBackground(cv::Mat img, cv::Rect face_rect)
{
    left_background.x = 0;
    left_background.y = 0;
    left_background.width = 0.25 * (face_rect.x);
    left_background.height = 0.25 * (face_rect.y);
}

void identifierface::findRightBackground(cv::Mat img, cv::Rect face_rect)
{
    right_background.width = 0.25 * (img.cols - (face_rect.x + face_rect.width));
    right_background.height = 0.25 * (face_rect.y);
    right_background.x = img.cols -1 - right_background.width;
    right_background.y = 0;
}

void identifierface::calculeUniformBackground(cv::Rect faceImage, cv::Mat img, int delta, int max)
{
    float deltaHead = 0.15f;
    double average = 0.0;
    double variance = 0.0;
    unsigned int count = 0.0;

    cv::Mat imgFloat, imgAverage,  imgVariance;;
    cv::Mat imgCheck = img.clone();

    img.convertTo(imgFloat, CV_32F);
    img.convertTo(imgAverage, CV_32F);
    img.convertTo(imgVariance, CV_32F);

    int x, w, y, h;
    int xcenter = img.cols / 2;
    x = faceImage.x - deltaHead * faceImage.width;

    if(x < 0)  x=1;

    w = x + faceImage.width + 2 * deltaHead * faceImage.width;

    if(w > img.cols)  w = img.cols - 1;

    y = faceImage.y - deltaHead * faceImage.height;

    if (y < 0) y = 1;

    h = y + faceImage.height + 2 * deltaHead * faceImage.height;

    if (h > img.rows) h = img.rows - 1;

    for (int k = 0; k < img.rows; k++)
    {
        for (int j = 0; j < img.cols; j++)
        {
            if (!((j >= x && j < w && k >= y &&  k < h ) || (k >= h ) || (k <= y && j >= x && j <= w)))
            {
                average+= imgFloat.at<float>(k, j);
                count++;
            }
        }
    }

    average /= count;

    for (int k = 0; k < img.rows; k++)
    {
        for (int j = 0; j < img.cols; j++)
        {
            if (!((j >= x && j < w && k >= y &&  k < h) || (k >= h) || (k <= y && j >= x && j <= w)))
            {
                variance += pow(imgFloat.at<float>(k, j) - average, 2);
                imgCheck.at<cv::Vec3b>(k, j)[0] = 0;   //B
                imgCheck.at<cv::Vec3b>(k, j)[1] = 0;   //G
                imgCheck.at<cv::Vec3b>(k, j)[2] = 0;   //R
            }
        }
    }

    variance /= (count - 1);

    for (int k = delta; k < img.rows - delta; k++)
    {
        for (int j = delta; j < img.cols - delta; j++)
        {
            imgAverage.at<float>(k, j) = 0;

            for (int m = - delta; m <= delta; m++)
            {
                for (int n = - delta; n <= delta; n++)
                {
                    imgAverage.at<float>(k, j) += imgFloat.at<float>(k + m, j + n);
                }
            }
            imgAverage.at<float>(k, j) /= pow((1 + 2 * delta), (float)2);;
        }
    }

    for (int k = delta; k < img.rows - delta; k++)
    {
        for (int j = delta; j < img.cols - delta; j++)
        {
            imgVariance.at<float>(k, j) = 0;

            for (int m = - delta; m <= delta; m++)
            {
                for (int n = - delta; n <= delta; n++)
                {
                    imgVariance.at<float>(k, j) += pow(imgFloat.at<float>(k + m, j + n) - imgAverage.at<float>(k, j), 2);
                }
            }
            imgVariance.at<float>(k, j) /= (pow(1 + 2 * delta, (float)2) - 1);
        }
    }

    float highVariance = 0.0;
    float noise = 0.0;

    for (int k = 1; k < img.rows-1; k++)
    {
        for (int j = 1; j < img.cols-1; j++)
        {
            if (!((j >= x && j < w && k >= y &&  k < h) || (k >= h) || (k <= y && j >= x && j <= w)))
            {

                if (imgVariance.at<float>(k, j) > max)
                {
                    highVariance = highVariance + 1;
                    noise = noise + (float)abs(xcenter - j) / xcenter;
                }
            }
        }
    }
    faceBackgroundUniform = (100.0 * noise) / count;
}*/
