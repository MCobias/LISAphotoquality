/**
*    Using some code from: https://github.com/trishume/eyeLike
*    http://thume.ca/projects/2012/11/04/simple-accurate-eye-center-tracking-in-opencv/
*/
#include <queue>
#include "Util.hpp"
#include "EyesCenterTracking.hpp"

using namespace std;

Mat *leftCornerKernel;
Mat *rightCornerKernel;

float kEyeCornerKernel[4][6] = {
    { -1,-1,-1, 1, 1, 1 },
    { -1,-1,-1,-1, 1, 1 },
    { -1,-1,-1,-1, 0, 3 },
    { 1, 1, 1, 1, 1, 1 },
};


EyesCenterTracking::EyesCenterTracking(){
}


EyesCenterTracking::~EyesCenterTracking(){
}

/**
* @details Busca subpixel do canto do olho.
*          Using some code from: https://github.com/trishume/eyeLike
*          http://thume.ca/projects/2012/11/04/simple-accurate-eye-center-tracking-in-opencv/
*/
Mat EyesCenterTracking::matrixMagnitude(const Mat &matX, const Mat &matY) {
    Mat mags(matX.rows, matX.cols, CV_64F);
    for (int y = 0; y < matX.rows; ++y) {
        const double *Xr = matX.ptr<double>(y), *Yr = matY.ptr<double>(y);
        double *Mr = mags.ptr<double>(y);
        for (int x = 0; x < matX.cols; ++x) {
            double gX = Xr[x], gY = Yr[x];
            double magnitude = sqrt((gX * gX) + (gY * gY));
            Mr[x] = magnitude;
        }
    }
    return mags;
}

/**
* @details Busca subpixel do canto do olho.
*          Using some code from: https://github.com/trishume/eyeLike
*          http://thume.ca/projects/2012/11/04/simple-accurate-eye-center-tracking-in-opencv/
*/
Mat EyesCenterTracking::computeMatXGradient(const Mat &mat) {
    Mat out(mat.rows, mat.cols, CV_64F);

    for (int y = 0; y < mat.rows; ++y) {
        const uchar *Mr = mat.ptr<uchar>(y);
        double *Or = out.ptr<double>(y);

        Or[0] = Mr[1] - Mr[0];
        for (int x = 1; x < mat.cols - 1; ++x) {
            Or[x] = (Mr[x + 1] - Mr[x - 1]) / 2.0;
        }
        Or[mat.cols - 1] = Mr[mat.cols - 1] - Mr[mat.cols - 2];
    }
    return out;
}

/**
* @details Busca subpixel do canto do olho.
*          Using some code from: https://github.com/trishume/eyeLike
*          http://thume.ca/projects/2012/11/04/simple-accurate-eye-center-tracking-in-opencv/
*/
double EyesCenterTracking::computeDynamicThreshold(const Mat &mat, double stdDevFactor) {
    Scalar stdMagnGrad, meanMagnGrad;
    meanStdDev(mat, meanMagnGrad, stdMagnGrad);
    double stdDev = stdMagnGrad[0] / sqrt(mat.rows*mat.cols);
    return stdDevFactor * stdDev + meanMagnGrad[0];
}

Mat EyesCenterTracking::setContournAndFind(Mat drawing, Point pt, vector<vector<Point> > contours, vector<Vec4i> hierarchy) {
    //Gerar contornos
    int valMaskAux = drawing.rows*0.10;
    int testvalMaskAux = drawing.rows*0.32;
    RNG rng(12345);
    for (int i = 0; i< contours.size(); i++)
    {
        if (contours[i].size() < testvalMaskAux) continue;
        if (this->existContourNearPointCenter(contours[i], pt, valMaskAux)) continue;
        Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
        drawContours(drawing, contours, i, color, 1, 8, hierarchy, 0, Point());
    }
    //imwrite("out/OUT_CANY" + fileName, drawing);
    return drawing;
}

/**
* @details Busca subpixel do canto do olho.
*          Using some code from: https://github.com/trishume/eyeLike
*          http://thume.ca/projects/2012/11/04/simple-accurate-eye-center-tracking-in-opencv/
*/
Point EyesCenterTracking::findEyeCenter(Mat eye)
{
    Point eyeCenter;
    Mat eyeGray = Util::coloredToGray(eye);

    double kGradientThreshold = 50.0;
    int kWeightBlurSize = 5;
    const float kPostProcessThreshold = 0.97;
    const bool kEnablePostProcess = true;
    const bool kPlotVectorField = true;

    Mat eyeROIUnscaled = eyeGray;

    //-- Find the gradient
    Mat gradientX = computeMatXGradient(eyeGray);
    Mat gradientY = computeMatXGradient(eyeGray.t()).t();

    //-- Normalize and threshold the gradient
    // compute all the magnitudes
    Mat mags = matrixMagnitude(gradientX, gradientY);
    //compute the threshold
    double gradientThresh = computeDynamicThreshold(mags, kGradientThreshold);
    //double gradientThresh = kGradientThreshold;
    //double gradientThresh = 0;
    //normalize
    for (int y = 0; y < eyeGray.rows; ++y) {
        double *Xr = gradientX.ptr<double>(y), *Yr = gradientY.ptr<double>(y);
        const double *Mr = mags.ptr<double>(y);
        for (int x = 0; x < eye.cols; ++x) {
            double gX = Xr[x], gY = Yr[x];
            double magnitude = Mr[x];
            if (magnitude > gradientThresh) {
                Xr[x] = gX / magnitude;
                Yr[x] = gY / magnitude;
            }
            else {
                Xr[x] = 0.0;
                Yr[x] = 0.0;
            }
        }
    }

    //-- Create a blurred and inverted image for weighting
    Mat weight;
    GaussianBlur(eyeGray, weight, Size(kWeightBlurSize, kWeightBlurSize), 0, 0);
    for (int y = 0; y < weight.rows; ++y) {
        unsigned char *row = weight.ptr<unsigned char>(y);
        for (int x = 0; x < weight.cols; ++x) {
            row[x] = (255 - row[x]);
        }
    }
    //imshow(debugWindow,weight);
    //-- Run the algorithm!
    Mat outSum = Mat::zeros(eyeGray.rows, eyeGray.cols, CV_64F);
    // for each possible gradient location
    // Note: these loops are reversed from the way the paper does them
    // it evaluates every possible center for each gradient location instead of
    // every possible gradient location for every center.
    //printf("Eye Size: %ix%i\n", outSum.cols, outSum.rows);

    for (int y = 0; y < weight.rows; ++y) {
        const double *Xr = gradientX.ptr<double>(y), *Yr = gradientY.ptr<double>(y);
        for (int x = 0; x < weight.cols; ++x) {
            double gX = Xr[x], gY = Yr[x];
            if (gX == 0.0 && gY == 0.0) {
                continue;
            }
            testPossibleCentersFormula(x, y, weight, gX, gY, outSum);
        }
    }
    // scale all the values down, basically averaging them
    double numGradients = (weight.rows*weight.cols);
    Mat out;
    outSum.convertTo(out, CV_32F, 1.0 / numGradients);
    //imshow(debugWindow,out);
    //-- Find the maximum point
    Point maxP;
    double maxVal;
    minMaxLoc(out, NULL, &maxVal, NULL, &maxP);
    //-- Flood fill the edges
    if (kEnablePostProcess) {
        Mat floodClone;
        //double floodThresh = computeDynamicThreshold(out, 1.5);
        double floodThresh = maxVal * kPostProcessThreshold;
        threshold(out, floodClone, floodThresh, 0.0f, THRESH_TOZERO);
        if (kPlotVectorField) {
            //plotVecField(gradientX, gradientY, floodClone);
            //imwrite("eyeFrame.png", eyeROIUnscaled);
        }
        Mat mask = floodKillEdges(floodClone);
        //imshow(debugWindow + " Mask",mask);
        //imshow(debugWindow,out);
        // redo max
        minMaxLoc(out, NULL, &maxVal, NULL, &maxP, mask);
    }
    //return unscalePoint(maxP, eye);
    return maxP;
}

/**
* @details Busca subpixel do canto do olho.
*          Using some code from: https://github.com/trishume/eyeLike
*          http://thume.ca/projects/2012/11/04/simple-accurate-eye-center-tracking-in-opencv/
*/
void EyesCenterTracking::testPossibleCentersFormula(int x, int y, const Mat &weight, double gx, double gy, Mat &out) {
    const float kWeightDivisor = 1.0;
    const bool kEnableWeight = true;
    // for all possible centers
    for (int cy = 0; cy < out.rows; ++cy) {
        double *Or = out.ptr<double>(cy);
        const unsigned char *Wr = weight.ptr<unsigned char>(cy);
        for (int cx = 0; cx < out.cols; ++cx) {
            if (x == cx && y == cy) {
                continue;
            }
            // create a vector from the possible center to the gradient origin
            double dx = x - cx;
            double dy = y - cy;
            // normalize d
            double magnitude = sqrt((dx * dx) + (dy * dy));
            dx = dx / magnitude;
            dy = dy / magnitude;
            double dotProduct = dx*gx + dy*gy;
            dotProduct = max(0.0, dotProduct);
            // square and multiply by the weight
            if (kEnableWeight) {
                Or[cx] += dotProduct * dotProduct * (Wr[cx] / kWeightDivisor);
            }
            else {
                Or[cx] += dotProduct * dotProduct;
            }
        }
    }
}

/**
* @details Busca subpixel do canto do olho.
*          Using some code from: https://github.com/trishume/eyeLike
*          http://thume.ca/projects/2012/11/04/simple-accurate-eye-center-tracking-in-opencv/
*/
bool EyesCenterTracking::inMat(Point p, int rows, int cols) {
    return p.x >= 0 && p.x < cols && p.y >= 0 && p.y < rows;
}

/**
* @details Busca subpixel do canto do olho.
*          Using some code from: https://github.com/trishume/eyeLike
*          http://thume.ca/projects/2012/11/04/simple-accurate-eye-center-tracking-in-opencv/
*/
bool EyesCenterTracking::floodShouldPushPoint(const Point &np, const Mat &mat) {
    return inMat(np, mat.rows, mat.cols);
}

/**
* @details Busca subpixel do canto do olho.
*          Using some code from: https://github.com/trishume/eyeLike
*          http://thume.ca/projects/2012/11/04/simple-accurate-eye-center-tracking-in-opencv/
*/
Mat EyesCenterTracking::floodKillEdges(Mat &mat) {
    rectangle(mat, Rect(0, 0, mat.cols, mat.rows), 255);

    Mat mask(mat.rows, mat.cols, CV_8U, 255);
    queue<Point> toDo;
    toDo.push(Point(0, 0));
    while (!toDo.empty()) {
        Point p = toDo.front();
        toDo.pop();
        if (mat.at<float>(p) == 0.0f) {
            continue;
        }
        // add in every direction
        Point np(p.x + 1, p.y); // right
        if (floodShouldPushPoint(np, mat)) toDo.push(np);
        np.x = p.x - 1; np.y = p.y; // left
        if (floodShouldPushPoint(np, mat)) toDo.push(np);
        np.x = p.x; np.y = p.y + 1; // down
        if (floodShouldPushPoint(np, mat)) toDo.push(np);
        np.x = p.x; np.y = p.y - 1; // up
        if (floodShouldPushPoint(np, mat)) toDo.push(np);
        // kill it
        mat.at<float>(p) = 0.0f;
        mask.at<uchar>(p) = 0;
    }
    return mask;
}

/**
* @details Busca subpixel do canto do olho.
*          Using some code from: https://github.com/trishume/eyeLike
*          http://thume.ca/projects/2012/11/04/simple-accurate-eye-center-tracking-in-opencv/
*/
void EyesCenterTracking::createCornerKernels() {
    rightCornerKernel = new cv::Mat(4, 6, CV_32F, kEyeCornerKernel);
    leftCornerKernel = new cv::Mat(4, 6, CV_32F);
    // flip horizontally
    cv::flip(*rightCornerKernel, *leftCornerKernel, 1);
}

/**
* @details Busca subpixel do canto do olho.
*          Using some code from: https://github.com/trishume/eyeLike
*          http://thume.ca/projects/2012/11/04/simple-accurate-eye-center-tracking-in-opencv/
*/
void EyesCenterTracking::releaseCornerKernels() {
    delete leftCornerKernel;
    delete rightCornerKernel;
}

/**
* @details Busca subpixel do canto do olho.
*          Using some code from: https://github.com/trishume/eyeLike
*          http://thume.ca/projects/2012/11/04/simple-accurate-eye-center-tracking-in-opencv/
*/
Mat EyesCenterTracking::eyeCornerMap(const cv::Mat &region, bool left, bool left2) {
    Mat cornerMap;

    Size sizeRegion = region.size();
    Range colRange(sizeRegion.width / 4, sizeRegion.width * 3 / 4);
    Range rowRange(sizeRegion.height / 4, sizeRegion.height * 3 / 4);

    Mat miRegion(region, rowRange, colRange);

    filter2D(miRegion, cornerMap, CV_32F,
        (left && !left2) || (!left && !left2) ? *leftCornerKernel : *rightCornerKernel);

    return cornerMap;
}

/**
* @details Busca subpixel do canto do olho.
*          Using some code from: https://github.com/trishume/eyeLike
*          http://thume.ca/projects/2012/11/04/simple-accurate-eye-center-tracking-in-opencv/
*/
Point2f EyesCenterTracking::findEyeCorner(Mat region, bool left, bool left2) {
    Mat cornerMap = eyeCornerMap(region, left, left2);

    Point maxP;
    minMaxLoc(cornerMap, NULL, NULL, NULL, &maxP);

    Point2f maxP2;
    maxP2 = findSubpixelEyeCorner(cornerMap, maxP);
    // GFTT
    //  std::vector<cv::Point2f> corners;
    //  cv::goodFeaturesToTrack(region, corners, 500, 0.005, 20);
    //  for (int i = 0; i < corners.size(); ++i) {
    //    cv::circle(region, corners[i], 2, 200);
    //  }
    //  imshow("Corners",region);

    return maxP2;
}

/**
* @details Busca subpixel do canto do olho.
*          Using some code from: https://github.com/trishume/eyeLike
*          http://thume.ca/projects/2012/11/04/simple-accurate-eye-center-tracking-in-opencv/
*/
Point2f EyesCenterTracking::findSubpixelEyeCorner(Mat region, Point maxP) {

    Size sizeRegion = region.size();

    // Matrix dichotomy
    // Not useful, matrix becomes too small

    /*int offsetX = 0;
    if(maxP.x - sizeRegion.width / 4 <= 0) {
    offsetX = 0;
    } else if(maxP.x + sizeRegion.width / 4 >= sizeRegion.width) {
    offsetX = sizeRegion.width / 2 - 1;
    } else {
    offsetX = maxP.x - sizeRegion.width / 4;
    }
    int offsetY = 0;
    if(maxP.y - sizeRegion.height / 4 <= 0) {
    offsetY = 0;
    } else if(maxP.y + sizeRegion.height / 4 >= sizeRegion.height) {
    offsetY = sizeRegion.height / 2 - 1;
    } else {
    offsetY = maxP.y - sizeRegion.height / 4;
    }
    cv::Range colRange(offsetX, offsetX + sizeRegion.width / 2);
    cv::Range rowRange(offsetY, offsetY + sizeRegion.height / 2);

    cv::Mat miRegion(region, rowRange, colRange);


    if(left){
    imshow("aa",miRegion);
    } else {
    imshow("aaa",miRegion);
    }*/

    Mat cornerMap(sizeRegion.height * 10, sizeRegion.width * 10, CV_32F);

    resize(region, cornerMap, cornerMap.size(), 0, 0, INTER_CUBIC);

    Point maxP2;
    minMaxLoc(cornerMap, NULL, NULL, NULL, &maxP2);

    return Point2f(sizeRegion.width / 2 + maxP2.x / 10,
        sizeRegion.height / 2 + maxP2.y / 10);
}


/**
* @brief Find right border after contourn.
*
* @details Busca a borda direita da pupila.
*
* @param img Contour image.
* @param pnt Center of Eye.
*
* @return right Border Point.
*/
Point EyesCenterTracking::findIrirBorderRight(Mat img, Point pnt) {
    cvtColor(img, img, CV_BGR2GRAY);
    Mat copy_img = img.clone();

    //Variables:
    int raio = 0;

    Point right(0, 0);

    Scalar colour;
    for (int x = pnt.x + 6; x < copy_img.cols; x++) {

        colour = copy_img.at<uchar>(Point(x, pnt.y));

        if (colour.val[0] > 0) {
            //achou borda direita da iris
            right = Point(x, pnt.y);
            break;
        }
    }
    return right;
}

//

/**
* @brief Find left border after contourn.
*
* @details Busca a borda esquerda da pupila.
*
* @param img Contour image.
* @param pnt Center of Eye.
*
* @return left Border Point.
*/
Point EyesCenterTracking::findIrirBorderLeft(Mat img, Point pnt) {
    cvtColor(img, img, CV_BGR2GRAY);
    Mat copy_img = img.clone();

    //Variables:
    int raio = 0;

    Point left(0, 0);

    Scalar colour;
    for (int x = pnt.x - 6; x > 0; x--) {

        colour = copy_img.at<uchar>(Point(x, pnt.y));

        if (colour.val[0] > 0) {
            //achou borda direita da iris
            left = Point(x, pnt.y);
            break;
        }
    }
    return left;
}

/**
* @brief Find contour points near center point.
*
* @details Busca se existe pontos de contorno no centro da pupila.
*          Estes pontos atrapalham no processo de identificacao das bordas.
*
* @param contourPoints List contour points.
* @param pnt Center of Eye.
* @param maskAux Value to create a range to find the points.
*
* @return existe Found or not found near points.
*/
bool EyesCenterTracking::existContourNearPointCenter(vector<Point> contourPoints, Point pnt, int maskAux) {
    bool existe = false;
    int rangeMintX = pnt.x - maskAux;
    int rangeMaxX  = rangeMintX + (maskAux * 2 );
    int rangeMintY = pnt.y - maskAux;
    int rangeMaxY = rangeMintY + (maskAux * 2 );

    Mat image(320, 240, CV_8UC3, Scalar(0, 0, 0));
    circle(image, pnt, 3, Scalar(0, 255, 0));

    //cout << "Ponto CENTRO: " << pnt << endl;
    for ( int i = 0; i < contourPoints.size(); i++ ) {
            if (contourPoints[i].x >= rangeMintX && contourPoints[i].x <= rangeMaxX &&
                contourPoints[i].y >= rangeMintY && contourPoints[i].y <= rangeMaxY) {
                //if (this->maxDistanceBtnPoints(contourPoints) > 15) continue;
                existe = true;
            }
            //cout << "Ponto: " << contourPoints[i] <<"X:"<< rangeMintX <<"<->"<< rangeMaxX << "| Y:" << rangeMintY << "<->" << rangeMaxY << " EXISTE: "<< existe << endl; circle(image, contourPoints[i], 0, Scalar(255, 255, 255));
            //cout << "Ponto: " << this->maxDistanceBtnPoints(contourPoints) <<"\n" << endl;
    }
    //imwrite("out/points.jpg", image);
    return existe;
}


/**
* @brief Find max distance between 2 points.
*
* @details Busca a maior distancia entre dois pontos do contorno.
*
* @param contourPoints List contour points.
*
* @return maxDist Pixel Distance.
*/
int EyesCenterTracking::maxDistanceBtnPoints(vector<Point> contourPoints) {
    int maxDist = 0;
    int dist = 0;
    for (int i = 0; i < contourPoints.size(); i++) {
        for (int j = 0; j < contourPoints.size(); j++) {
            if (i == j) continue;
            dist = Utils::getDistanceBtn2Pnts(contourPoints[i], contourPoints[j]);
            if (dist > maxDist) maxDist = dist;
        }
    }
    return maxDist;
}

/**
* @brief Identify if is eye pair region.
*
* @details Calcula a imagem binaria e busca a esclera do olho.
*
* @param img Eye pair image.
*
* @return res Is or not the Eye Pair region.
*/
bool EyesCenterTracking::isEyePairRegion(const Mat &img) {
    bool res = false;
    Mat bw;
    //imshow("normal", img);
    threshold(img, bw, 240, 255, CV_THRESH_BINARY_INV | CV_THRESH_OTSU);

    imwrite("teste.png", bw);

    int resVal = countNonZero(bw);


    cout << "Cloud all black pixels: " << resVal << endl; // amount of black pixels is returned from the size

                                                                       // Show binary image
    //imshow("binary", bw);
    //int totalPixelImg = img.cols * img.rows;
    //if (black_pixels.size() < (totalPixelImg*0.9)) res = true;
    return res;
}

/**
* @brief Verify circle.
*
*/
float EyesCenterTracking::verifyCircle(Mat dt, Point2f center, float radius, vector<Point2f> & inlierSet){
    unsigned int counter = 0;
    unsigned int inlier = 0;
    float minInlierDist = 2.0f;
    float maxInlierDistMax = 100.0f;
    float maxInlierDist = radius / 25.0f;
    if (maxInlierDist<minInlierDist) maxInlierDist = minInlierDist;
    if (maxInlierDist>maxInlierDistMax) maxInlierDist = maxInlierDistMax;

    // choose samples along the circle and count inlier percentage
    for (float t = 0; t<2 * 3.14159265359f; t += 0.05f) {
        counter++;
        float cX = radius*cos(t) + center.x;
        float cY = radius*sin(t) + center.y;

        if (cX < dt.cols)
            if (cX >= 0)
                if (cY < dt.rows)
                    if (cY >= 0)
                        if (dt.at<float>(cY, cX) < maxInlierDist)
                        {
                            inlier++;
                            inlierSet.push_back(cv::Point2f(cX, cY));
                        }
    }

    return (float)inlier / float(counter);
}

/**
* @brief Get circle.
*
* @details get circle from points.
*
* @param p1 Point 1.
* @param p2 Point 2.
* @param p3 Point 3.
* @param center center of point.
* @param radius circle radius.
*
*/
void EyesCenterTracking::getCircle(Point2f& p1, Point2f& p2, Point2f& p3, Point2f& center, float& radius){
    float x1 = p1.x;
    float x2 = p2.x;
    float x3 = p3.x;

    float y1 = p1.y;
    float y2 = p2.y;
    float y3 = p3.y;

    // PLEASE CHECK FOR TYPOS IN THE FORMULA :)
    center.x = (x1*x1 + y1*y1)*(y2 - y3) + (x2*x2 + y2*y2)*(y3 - y1) + (x3*x3 + y3*y3)*(y1 - y2);
    center.x /= (2 * (x1*(y2 - y3) - y1*(x2 - x3) + x2*y3 - x3*y2));

    center.y = (x1*x1 + y1*y1)*(x3 - x2) + (x2*x2 + y2*y2)*(x1 - x3) + (x3*x3 + y3*y3)*(x2 - x1);
    center.y /= (2 * (x1*(y2 - y3) - y1*(x2 - x3) + x2*y3 - x3*y2));

    radius = sqrt((center.x - x1)*(center.x - x1) + (center.y - y1)*(center.y - y1));
}


/**
* @brief Get position points.
*
* @details get vector points.
*
* @param binaryImage Binary image.

* @return pointPositions Vector Points.
*
*/
vector<Point2f> EyesCenterTracking::getPointPositions(Mat binaryImage)
{
    vector<Point2f> pointPositions;

    for (unsigned int y = 0; y<binaryImage.rows; ++y)
    {
        //unsigned char* rowPtr = binaryImage.ptr<unsigned char>(y);
        for (unsigned int x = 0; x<binaryImage.cols; ++x)
        {
            //if(rowPtr[x] > 0) pointPositions.push_back(cv::Point2i(x,y));
            if (binaryImage.at<unsigned char>(y, x) > 0) pointPositions.push_back(Point2f(x, y));
        }
    }

    return pointPositions;
}

/**
* @brief Get points of circles intersections.
*
* @details Funcao para pegar o ponto superior de interseccao de dois circulos.
*          Baseado: http://paulbourke.net/geometry/circlesphere/tvoght.c
*
* @param p0 First point.
* @param r0 Radius of first point.
* @param p1 Second point.
* @param r1 Radius of second point.
*
* @return pRes Superior point inrtersection.
*
*/

Point2f EyesCenterTracking::getCirclesIntersection(Point2f p0, float r0, Point2f p1, float r1) {
    Point2f pRes = Point2f(0, 0);

    double a, dx, dy, d, h, rx, ry;
    double x2, y2;

    //dx and dy are the vertical and horizontal distances between
    //the circle centers.
    dx = p1.x - p0.x;
    dy = p1.y - p0.y;

    //Determine the straight-line distance between the centers.
    d = hypot(dx, dy); // Suggested by Keith Briggs

                       //Check for solvability.
                       // if true no solution. circles do not intersect.
    if (d > (r0 + r1) == false) {
        // if true: one circle is contained in the other
        if (d < fabs(r0 - r1) == false) {

            //Determine the distance from point 0 to point 2.
            a = ((r0*r0) - (r1*r1) + (d*d)) / (2.0 * d);

            //Determine the coordinates of point 2.
            Point2f p2;
            p2.x = p0.x + a*(p1.x - p0.x) / d;
            p2.y = p0.y + a*(p1.y - p0.y) / d;

            //Determine the distance from point 2 to either of the
            //intersection points.
            h = sqrt((r0*r0) - (a*a));

            //Now determine the offsets of the intersection points from
            //point 2.
            rx = -dy * (h / d);
            ry = dx * (h / d);

            //Determine the absolute intersection points.
            Point2f p3a, p3b;
            p3a.x = p2.x + rx;
            p3b.x = p2.x - rx;
            p3a.y = p2.y + ry;
            p3b.y = p2.y - ry;

            //Ponto superior p3b
            pRes = p3b;
        }
    }
    return pRes;
}

/**
* @brief Filling holes in a binary image.
*
* @details Funcao preencher os buracos em uma imagem binaria (buracos = brilhos do flash).
*
* @param binaryImage Image.
*
* @return result Image processed.
*
*/
Mat EyesCenterTracking::fillHoles(Mat GrayImage) {
    cv::normalize(GrayImage, GrayImage, 0, 1, cv::NORM_MINMAX);

    cv::Mat dst;
    dst = cv::Mat::zeros(GrayImage.size(), CV_8U);
    dst.at<uchar>((int)GrayImage.rows/2, (int)GrayImage.rows / 2) = 1;

    cv::Mat prev;
    cv::Mat kernel = (cv::Mat_<uchar>(3, 3) << 0, 1, 0, 1, 1, 1, 0, 1, 0);

    do {
        dst.copyTo(prev);
        cv::dilate(dst, dst, kernel);
        dst &= (1 - GrayImage);
    } while (cv::countNonZero(dst - prev) > 0);

    cv::normalize(GrayImage, GrayImage, 0, 255, cv::NORM_MINMAX);
    cv::normalize(dst, dst, 0, 255, cv::NORM_MINMAX);

    return GrayImage;
}

/**
* @brief Find extreme white pixel.
*
* @details Identificar o pixel mais extremo no eixo X.
*
* @param img Binary Image.
* @param isLef Bool aux.
*
* @return res Pixel Position.
*
*/
Point2f EyesCenterTracking::getMostWhitePixel(Mat img, bool isLeft) {
    threshold(img, img, 40, 255, CV_THRESH_BINARY_INV | CV_THRESH_OTSU);
    //imwrite("out_DEBUG/LEFT_eye-LEFT_CONER_BORDER.png", img);

    Point2f res = Point2f(0, 0);
    Scalar intensity;
    bool crtAux = false;
    if (isLeft) {
        for (int j = 0; j < img.cols; j++){
            for (int i = 0; i < img.rows; i++){
                intensity = img.at<uchar>(Point(j, i));
                //cout << "("<<j << ","<< i <<") = "<< intensity.val[0] << endl;
                if (intensity.val[0] > 0) continue;
                res = Point2f(j, i);
                crtAux = true;
                break;
            }
            if (crtAux) break;
        }
    }
    else {
        for (int j = img.cols - 1; j >= 0; j--) {
            for (int i = 0; i < img.rows; i++) {
                intensity = img.at<uchar>(Point(j, i));
                //cout << "(" << j << "," << i << ") = " << intensity.val[0] << endl;
                if (intensity.val[0] > 0) continue;
                res = Point2f(j, i);
                crtAux = true;
                break;

            }
            if (crtAux) break;
        }
    }

    return res;
}
