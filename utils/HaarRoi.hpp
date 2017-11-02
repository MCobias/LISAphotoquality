//
//  imagetester.cpp
//  ICAOphotoquality
//
//  Copyright © 2017 Marcelo Cobias. All rights reserved.
//
#ifndef HaarRoi_HPP
#define HaarRoi_HPP

#include <string>
#include <opencv2/opencv.hpp>
#include "Util.hpp"

using namespace std;
using namespace cv;

class HaarRoi
{
public:
    HaarRoi(Mat image);
    Rect_<double> getRoiFace();
    Rect_<double> getRoiEyersPair();
    Rect_<double> getRoiEyeLeft();
    Rect_<double> getRoiEyeRight();
    ~HaarRoi();
private:
    CascadeClassifier faceCascade, eyePairCascade;
    Rect_<double> rectEyeLeft, rectEyeRight;
    Rect_<double> face, eyePair, customROI;
    Rect_<double> findEyesPair(Mat face);
    Rect_<double> findFace(Mat image);
    void loadCascadeClassifier();
    void calculeEyeSingle(Mat imageFace, Rect eyePair);
};
#endif
