// System includes
#include <iostream>

// OpenCV includes
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

// Dlib includes
#include <dlib/image_processing/frontal_face_detector.h>
#include <tbb/tbb.h>

//Local includes
#include "utils/Utils.hpp"
#include "utils/ImageDegradation.hpp"
#include "utils/EyesCenterTracking.hpp"
#include "utils/HaarRoi.hpp"
#include "ImageAttributes.hpp"

using namespace std;
using namespace cv;

int main(int argc, char const *argv[])
{
  Mat test;
  test = imread("./img/m-001-1.jpg");

  ImageAttributes atr = ImageAttributes(test);
  EyesCenterTracking eye = EyesCenterTracking();
  Utils::showHistogram(test);

  imshow("Hello Word!", test);

  HaarRoi elemets = HaarRoi(test);
  Mat face = Utils::cutImage(test, elemets.getRoiFace());
  Mat eyes = Utils::cutImage(face, elemets.getRoiEyersPair());

  Mat right = Utils::cutImage(eyes, elemets.getRoiEyeRight());
  Mat left = Utils::cutImage(eyes, elemets.getRoiEyeLeft());

  Point leftEyePoint = eye.findEyeCenter(left);
  Point rightEyePoint = eye.findEyeCenter(right);

  circle(left, leftEyePoint, 1, Scalar(255, 0, 0));
  circle(right, rightEyePoint, 1, Scalar(255, 0, 0));

  imshow("Hello Word!1", right);
  imshow("Hello Word!2", left);
  cvWaitKey(0);

  return EXIT_SUCCESS;
}
