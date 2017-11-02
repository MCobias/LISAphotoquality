//
//  ICAOphotoquality
//
//  Copyright © 2017 Marcelo Cobias. All rights reserved.
//
#ifndef FindLandmark_HPP
#define FindLandmark_HPP

#include <iostream>
#include <opencv2/imgproc.hpp>

#include "Util.hpp"
#include <dlib/opencv.h>
#include <dlib/image_processing.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing/shape_predictor.h>

const float MAXFACEANGLE = 75.0f;

class FindLandmark
{
public:
  void loadShapePredictor();
  FindLandmark(cv::Mat image, cv::Rect rectFace);
  cv::Point getFaceLeftEye();
  cv::Point getFaceRightEye();
  cv::Point3f getFaceApproxEulerAngles();
  cv::Rect getFaceBBoxFromLandmarks(std::vector<cv::Point> landmarks, cv::Size imageSize, bool square);
  bool printLandmarks(cv::Mat &image, std::vector<cv::Point> landmarks, int thickness);
  std::vector<cv::Point> getLandmark();
  ~FindLandmark();
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
