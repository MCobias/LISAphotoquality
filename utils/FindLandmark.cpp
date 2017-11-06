/**
*    Using some code from: https://github.com/YuvalNirkin/find_face_landmarks
*/
#include "FindLandmark.hpp"

void FindLandmark::loadShapePredictor()
{
  if(ifstream("./data/shape_predictor_68_face_landmarks.dat"))
     dlib::deserialize("./data/shape_predictor_68_face_landmarks.dat") >> landmark_model;
  else
      std::cout << "(!) Error Loading shape predictor!" << '\n';
}

FindLandmark::FindLandmark(cv::Mat image, cv::Rect rectFace)
{
  loadShapePredictor();
  this->landmarksFace = extractPoints(image, rectFace);
}

static dlib::rectangle opencvRectToDlib(cv::Rect form)
{
  return dlib::rectangle((long)form.tl().x, (long)form.tl().y, (long)form.br().x - 1, (long)form.br().y - 1);
}

void FindLandmark::dlibObjToPoints(const dlib::full_object_detection& obj, std::vector<cv::Point>& points)
{
  points.resize(obj.num_parts());
  for (unsigned long i = 0; i < obj.num_parts(); i++)
  {
      cv::Point& p = points[i];
      const dlib::point& objPoints = obj.part(i);
      p.x = (float)objPoints.x();
      p.y = (float)objPoints.y();
  }
}

std::vector<cv::Point> FindLandmark::extractPoints(cv::Mat image, cv::Rect rectFace)
{
  dlib::cv_image<dlib::bgr_pixel> cimage(image);
  std::vector<cv::Point> pointsFace;
  dlib::full_object_detection shapes;
  shapes = landmark_model(cimage, opencvRectToDlib(rectFace));
  dlibObjToPoints(shapes, pointsFace);
  return pointsFace;
}

bool FindLandmark::printLandmarks(cv::Mat &image, int thickness)
{
  if (landmarksFace.size() != 68)
    return false;

	for(size_t i = 1; i <= 16; i++)
		cv::line(image, landmarksFace[i], landmarksFace[i - 1], cv::Scalar(0, 0, 255), thickness);

	for(size_t i = 28; i <= 30; i++)
		cv::line(image, landmarksFace[i], landmarksFace[i - 1], cv::Scalar(0, 0, 255), thickness);

	for(size_t i = 18; i <= 21; i++)
		cv::line(image, landmarksFace[i], landmarksFace[i - 1], cv::Scalar(0, 0, 255), thickness);

	for(size_t i = 23; i <= 26; i++)
		cv::line(image, landmarksFace[i], landmarksFace[i - 1], cv::Scalar(0, 0, 255), thickness);

	for(size_t i = 31; i <= 35; i++)
		cv::line(image, landmarksFace[i], landmarksFace[i - 1], cv::Scalar(0, 0, 255), thickness);
	cv::line(image, landmarksFace[30], landmarksFace[35], cv::Scalar(0, 0, 255), thickness);

	for(size_t i = 37; i <= 41; i++)
		cv::line(image, landmarksFace[i], landmarksFace[i - 1], cv::Scalar(0, 0, 255), thickness);
	cv::line(image, landmarksFace[36], landmarksFace[41], cv::Scalar(0, 0, 255), thickness);

	for(size_t i = 43; i <= 47; i++)
		cv::line(image, landmarksFace[i], landmarksFace[i - 1], cv::Scalar(0, 0, 255), thickness);
	cv::line(image, landmarksFace[42], landmarksFace[47], cv::Scalar(0, 0, 255), thickness);

	for(size_t i = 49; i <= 59; i++)
		cv::line(image, landmarksFace[i], landmarksFace[i - 1], cv::Scalar(0, 0, 255), thickness);
	cv::line(image, landmarksFace[48], landmarksFace[59], cv::Scalar(0, 0, 255), thickness);

	for (size_t i = 61; i <= 67; i++)
		cv::line(image, landmarksFace[i], landmarksFace[i - 1], cv::Scalar(0, 0, 255), thickness);
	cv::line(image, landmarksFace[60], landmarksFace[67], cv::Scalar(0, 0, 255), thickness);

    for(size_t i = 0; i < landmarksFace.size(); i++)
        cv::circle(image, landmarksFace[i], 2, Scalar(0, 255, 0));

    cv::Point eyeLeftCenter = getLeftCenterEye();
    cv::circle(image, eyeLeftCenter, 2, Scalar(0, 165, 255));

    cv::Point eyeRightCenter = getRightCenterEye();
    cv::circle(image, eyeRightCenter, 2, Scalar(0, 165, 255));

	for(size_t i = 0; i < landmarksFace.size(); i++)
		cv::putText(image, Util::intToStr(i+1), landmarksFace[i], FONT_HERSHEY_PLAIN, 0.5, cv::Scalar(0, 0, 0), thickness);

    cv::putText(image, Util::intToStr(landmarksFace.size()+2), eyeLeftCenter, FONT_HERSHEY_PLAIN, 0.5, cv::Scalar(0, 0, 0), thickness);
    cv::putText(image, Util::intToStr(landmarksFace.size()+3), eyeRightCenter, FONT_HERSHEY_PLAIN, 0.5, cv::Scalar(0, 0, 0), thickness);
  return true;
}

cv::Point FindLandmark::getLeftCenterEye()
{
  if (landmarksFace.size() != 68) return Point2f();

  cv::Point leftEye(0, 0);
  for (size_t i = 42; i <= 47; i++)
    leftEye += Point(landmarksFace[i]);

  return (leftEye / 6);
}

cv::Point FindLandmark::getRightCenterEye()
{
  if (landmarksFace.size() != 68) return Point2f();

  cv::Point rightEye(0, 0);
  for (size_t i = 36; i <= 41; i++)
    rightEye += Point(landmarksFace[i]);

  return (rightEye / 6);
}

std::vector<cv::Point> FindLandmark::getMouth()
{
    if (landmarksFace.size() != 68) return std::vector<cv::Point>();

    std::vector<cv::Point> mouthPoints;
    for (size_t i = 48; i <= 67; i++)
     mouthPoints.push_back(landmarksFace[i]);

    return mouthPoints;
}

float FindLandmark::getFaceApproxVertAngle()
{
  if (landmarksFace.size() != 68) return 0;

  cv::Point leftEye = getLeftCenterEye();
  cv::Point rightEye = getRightCenterEye();
  cv::Point x1 = landmarksFace[0], x2 = landmarksFace[16];
  cv::Point v = x2 - x1;
  cv::Point leftEyeDir = x1 - leftEye;
  cv::Point rightEyeDir = x1 - rightEye;
  float x12Dist = norm(v);
  float d1 = v.cross(rightEyeDir) / x12Dist;
  float d2 = v.cross(leftEyeDir) / x12Dist;
  float d = (d1 + d2)*0.5f / norm(leftEye - rightEye);
  return d * (2 * MAXFACEANGLE) * (CV_PI / 180.0f);
}

float FindLandmark::getFaceApproxHorAngle()
{
  if (landmarksFace.size() != 68) return 0;

  const float maxAngle = 75.0f;
  const cv::Point& center = landmarksFace[27];
  const cv::Point& rightEye = landmarksFace[39];
  const cv::Point& leftEye = landmarksFace[42];
  float leftDist = norm(center - leftEye);
  float rightDist = norm(center - rightEye);
  float d = (leftDist / (leftDist + rightDist) - 0.5f);
  return d * (2 * MAXFACEANGLE) * (CV_PI / 180.0f);
}

float FindLandmark::getFaceApproxTiltAngle()
{
  if (landmarksFace.size() != 68) return 0;

  cv::Point leftEye = getLeftCenterEye();
  cv::Point rightEye = getRightCenterEye();
  cv::Point v = leftEye - rightEye;
  return atan2(v.y, v.x);
}

cv::Point3f FindLandmark::getFaceApproxEulerAngles()
{
  float x = getFaceApproxVertAngle();
  float y = getFaceApproxHorAngle();
  float z = getFaceApproxTiltAngle();

  return cv::Point3f(x, y, z);
}

cv::Rect FindLandmark::getFaceBBoxFromLandmarks(std::vector<cv::Point> landmarks, cv::Size imageSize, bool square)
{
  int xmin(numeric_limits<int>::max()), ymin(numeric_limits<int>::max()), xmax(-1), ymax(-1), sumx(0), sumy(0);
  for (const cv::Point& p : landmarks)
  {
    xmin = min(xmin, p.x);
    ymin = min(ymin, p.y);
    xmax = max(xmax, p.x);
    ymax = max(ymax, p.y);
    sumx += p.x;
    sumy += p.y;
  }

  int width = xmax - xmin + 1;
  int height = ymax - ymin + 1;
  int centerx = (xmin + xmax) / 2;
  int centery = (ymin + ymax) / 2;
  int avgx = (int)round(sumx / landmarks.size());
  int avgy = (int)round(sumy / landmarks.size());
  int devx = centerx - avgx;
  int devy = centery - avgy;
  int dleft = (int)round(0.1*width) + abs(devx < 0 ? devx : 0);
  int dtop = (int)round(height*(max(float(width) / height, 1.0f) * 2 - 1)) + abs(devy < 0 ? devy : 0);
  int dright = (int)round(0.1*width) + abs(devx > 0 ? devx : 0);
  int dbottom = (int)round(0.1*height) + abs(devy > 0 ? devy : 0);

  // Limit to frame boundaries
  xmin = max(0, xmin - dleft);
  ymin = max(0, ymin - dtop);
  xmax = min((int)imageSize.width - 1, xmax + dright);
  ymax = min((int)imageSize.height - 1, ymax + dbottom);

  // Make square
  if (square)
  {
    int sq_width = max(xmax - xmin + 1, ymax - ymin + 1);
    centerx = (xmin + xmax) / 2;
    centery = (ymin + ymax) / 2;
    xmin = centerx - ((sq_width - 1) / 2);
    ymin = centery - ((sq_width - 1) / 2);
    xmax = xmin + sq_width - 1;
    ymax = ymin + sq_width - 1;

    // Limit to frame boundaries
    xmin = max(0, xmin);
    ymin = max(0, ymin);
    xmax = min((int)imageSize.width - 1, xmax);
    ymax = min((int)imageSize.height - 1, ymax);
  }
  return cv::Rect(Point(xmin, ymin), cv::Point(xmax, ymax));
}

std::vector<cv::Point> FindLandmark::getLandmark()
{
  return this->landmarksFace;
}

FindLandmark::~FindLandmark(){}
