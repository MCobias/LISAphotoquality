// System includes
#include <iostream>
// OpenCV includes
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
//Local includes
#include "utils/Util.hpp"
#include "utils/ImageDegradation.hpp"
#include "utils/EyesCenterTracking.hpp"
#include "utils/FindLandmark.hpp"
#include "utils/HaarRoi.hpp"
#include "ImageAttribute.hpp"

using namespace std;
using namespace cv;

int main(int argc, char const *argv[])
{
  Mat test;
  test = imread("./img/00003_940307_fa_a.jpg");

  ImageAttribute atr = ImageAttribute(test);
  EyesCenterTracking eye = EyesCenterTracking();
  Util::showHistogram(test);

  HaarRoi elemets = HaarRoi(test);
  Mat face = Util::cutImage(test, elemets.getRoiFace());
  imshow("Hello Word!", face);

  FindLandmark land = FindLandmark(test, elemets.getRoiFace());
  if(land.printLandmarks(test, land.getLandmark(), 1))
      imshow("Hello Word!1", test);
  //Mat eyes = Util::cutImage(face, elemets.getRoiEyersPair());

  //Mat right = Util::cutImage(eyes, elemets.getRoiEyeRight());
  //Mat left = Util::cutImage(eyes, elemets.getRoiEyeLeft());

  //Point leftEyePoint = eye.findEyeCenter(left);
  //Point rightEyePoint = eye.findEyeCenter(right);

  //Point cornerleftEyePoint = eye.findIrirBorderLeft(left, leftEyePoint);
  //Point cornerrightEyePoint = eye.findIrirBorderRight(right, rightEyePoint);

  //circle(left, leftEyePoint, 1, Scalar(255, 0, 0));
  //circle(left, cornerleftEyePoint, 1, Scalar(255, 0, 0));
  //circle(right, rightEyePoint, 1, Scalar(255, 0, 0));
  //circle(right, cornerrightEyePoint, 1, Scalar(255, 0, 0));


  //imshow("Hello Word!2", left);
  cvWaitKey(0);

  return EXIT_SUCCESS;
}
