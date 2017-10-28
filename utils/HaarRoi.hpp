/**
*    Copyright (C) 2016 Lucas Porto, Flávio Vidal.
*/

#ifndef HaarRoi_HPP
#define HaarRoi_HPP

#include <string>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

class HaarRoi
{
public:
    HaarRoi(Mat image);
    Rect getCunstomRoi();
    ~HaarRoi();
private:
    CascadeClassifier face_cascade, eye_pair_cascade;
    Rect face, eyePair, customROI;
    Rect findEyesPair(Mat face);
    Rect findFace(Mat image);
    Rect getFace(Mat image);
    bool faceIsCentralized();
    void loadCascadeClassifier();
    bool faceIsCentralized(Rect mainImg, Rect face);
};
#endif
