//
//  ICAOphotoquality
//
//  Copyright © 2017 Marcelo Cobias. All rights reserved.
//
#ifndef LandmarkFind_HPP
#define LandmarkFind_HPP

#include <iostream>
#include <opencv2/imgproc.hpp>

#include "Util.hpp"
#include <dlib/opencv.h>
#include <dlib/image_processing.h>
#include <dlib/image_processing/shape_predictor.h>

const float MAXFACEANGLE = 75.0f;

class LandmarkFind
{
public:
  void loadShapePredictor();
  LandmarkFind(cv::Mat image, cv::Rect rectFace);
  cv::Point getLeftCenterEye();
  cv::Point getRightCenterEye();
  std::vector<cv::Point> getMouth();
  cv::Point3f getFaceApproxEulerAngles();
  cv::Rect getFaceBBoxFromLandmarks(std::vector<cv::Point> landmarks, cv::Size imageSize, bool square);
  bool printLandmarks(cv::Mat &image, int thickness);
  bool printLandmarksZero(cv::Mat &image, int thickness);
  std::vector<cv::Point> getLandmark();
  ~LandmarkFind();
private:
  std::vector<cv::Point> landmarksFace;
  dlib::shape_predictor landmark_model;
  void dlibObjToPoints(const dlib::full_object_detection& obj, std::vector<cv::Point>& points);
  std::vector<cv::Point> extractPoints(cv::Mat image, cv::Rect rectFace);
  float getFaceApproxTiltAngle();
  float getFaceApproxHorAngle();
  float getFaceApproxVertAngle();
};
#endif
