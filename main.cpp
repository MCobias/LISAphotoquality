// System includes
#include <iostream>
#include <fstream>

// OpenCV includes
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

// Boost includes
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

// Dlib includes
#include <dlib/image_processing/frontal_face_detector.h>
#include <tbb/tbb.h>

#include "utils/utils.hpp"
#include "imageAttributes.hpp"

using namespace std;
using namespace cv;

int main(int argc, char const *argv[])
{
  Mat test;
  test = imread("./img/m-001-1.jpg");

  imageAttributes atr = imageAttributes(test);

  imshow("Hello Word!", test);
  cvWaitKey(0);

  return EXIT_SUCCESS;
}
