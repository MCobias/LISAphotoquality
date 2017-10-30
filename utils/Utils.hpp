//
//  ICAOphotoquality
//
//  Copyright © 2017 Marcelo Cobias. All rights reserved.
//
#ifndef Utils_HPP
#define Utils_HPP

#include <fstream>
#include <sys/stat.h>
#include <math.h>
#include <time.h>
#include <iterator>
#include <vector>
#include <glob.h>
#include <utility>
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class Utils
{
public:
  static bool existsImage(Mat image);
  static Mat cutImage(Mat image, Rect form);
  static float getDistanceBtn2Pnts(Point2f p1, Point2f p2);
  static Mat coloredToGray(Mat image);
  vector<string> splitAtCommas(const string& row);
  string intToStr(int val);
  string floToStr(float val);
  void replaceStringInPlace(string& subject, const string& search, const string& replace);
  float srtToNumber(string &Text);
  bool isValidImage(string imgName);
  vector<string> divStr(string const & s, char delim);
  vector<string> getListPathFilesImage(string path);
  bool isDirExist(string path);
  bool makePath(string path);
  bool intersecPoints(Point2f o1, Point2f p1, Point2f o2, Point2f p2, Point2f &r);
  void glcm(Mat &img, vector<float> &values);
  static void showHistogram(Mat& img);
  Vec3b getColorSubpix(const Mat& img, Point2f pt);

};
#endif
