//
//  imagetester.cpp
//  ICAOphotoquality
//
//  Copyright © 2017 Marcelo Cobias. All rights reserved.
//
#ifndef EyesCenterTracking_HPP
#define EyesCenterTracking_HPP

#include "Util.hpp"
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

#define kEyeLeft true
#define kEyeRight false

class EyesCenterTracking
{
public:
    EyesCenterTracking();
    ~EyesCenterTracking();
    Point findEyeCenter(Mat eyeImage);
    void createCornerKernels();
    void releaseCornerKernels();
    Point2f findEyeCorner(Mat region, bool left, bool left2);
    double computeDynamicThreshold(const Mat &mat, double stdDevFactor);
    Point findIrirBorderRight(Mat img, Point pnt);
    Point findIrirBorderLeft(Mat img, Point pnt);
    bool existContourNearPointCenter(vector<Point> contourPoints, Point pnt, int maskAux = 6);
    int maxDistanceBtnPoints(vector<Point> contourPoints);
    Mat setContournAndFind(Mat drawing, Point pt, vector<vector<Point> > contours, vector<Vec4i> hierarchy);
    Mat fillHoles(Mat binaryImage);
    bool isEyePairRegion(const Mat &img);
    float verifyCircle(Mat dt, Point2f center, float radius, vector<Point2f> & inlierSet);
    void getCircle(Point2f& p1, Point2f& p2, Point2f& p3, Point2f& center, float& radius);
    vector<Point2f> getPointPositions(Mat binaryImage);
    Point2f getCirclesIntersection(Point2f p0, float r0, Point2f p1, float r1);
    Point2f getMostWhitePixel(Mat GrayImage, bool isLeft);
private:
    Mat floodKillEdges(Mat &mat);
    Mat computeMatXGradient(const Mat &mat);
    Mat matrixMagnitude(const Mat &matX, const Mat &matY);
    void testPossibleCentersFormula(int x, int y, const Mat &weight, double gx, double gy, Mat &out);
    bool floodShouldPushPoint(const Point &np, const Mat &mat);
    bool inMat(Point p, int rows, int cols);

    Point2f findSubpixelEyeCorner(Mat region, Point maxP);
    Mat eyeCornerMap(const Mat &region, bool left, bool left2);
};

#endif
