// System includes
#include <iostream>
// OpenCV includes
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
//Local includes
#include "utils/Util.hpp"
#include "utils/ImageDegradation.hpp"
#include "utils/EyesFindCenter.hpp"
#include "utils/LandmarkFind.hpp"
#include "utils/HaarFind.hpp"
#include "ImageAttribute.hpp"

using namespace std;
using namespace cv;

/**
* @ Normalize image colored to gray
*/
bool normImage(Mat &image)
{
    if(image.channels() == 3)
        image = Util::coloredToGray(image);
    return true;
}

/*=================================================================*/
void test()
{
  Mat test;
  test = imread("./img/glasses/01.jpg");
  HaarFind elemets = HaarFind(test);
  Mat face = Util::cutImage(test, elemets.getRoiFace());
  imshow("Face: ", face);

  LandmarkFind land = LandmarkFind(test, elemets.getRoiFace());
  if(land.printLandmarksZero(test, 1))
      imshow("Landmark: ", test);
  cvWaitKey(0);
}

void test2()
{
  Mat test;
  test = imread("./img/good/01.jpg");
  EyesFindCenter eye = EyesFindCenter();

  HaarFind elemets = HaarFind(test);
  Mat face = Util::cutImage(test, elemets.getRoiFace());
  imshow("Face: ", face);

  Mat eyePair = Util::cutImage(face, elemets.getRoiEyersPair());
  imshow("Eye pair: ", eyePair);

  Mat eyeRight = Util::cutImage(eyePair, elemets.getRoiEyeRight());
  Mat eyeLeft = Util::cutImage(eyePair, elemets.getRoiEyeLeft());

  Point right = eye.findEyeCenter(eyeRight);
  Point left = eye.findEyeCenter(eyeLeft);
  cv::circle(eyeRight, right, 2, Scalar(0, 255, 0));
  cv::circle(eyeLeft, left, 2, Scalar(0, 255, 0));

  imshow("Eye right: ", eyeRight);
  imshow("Eye left: ", eyeLeft);
  cvWaitKey(0);
}
/*=================================================================*/

int main(int argc, char const *argv[])
{
  test();
  return EXIT_SUCCESS;
}
