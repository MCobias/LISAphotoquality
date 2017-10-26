//
//  ICAOphotoquality
//
//  Copyright © 2017 Marcelo Cobias. All rights reserved.
//

#ifndef Utils_HPP
#define Utils_HPP

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

class Utils
{
public:
  static bool existsImage(cv::Mat image);
  static cv::Mat cutImage(cv::Mat image, cv::Rect form);

};
#endif
