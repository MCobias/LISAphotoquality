//
//  imagetester.cpp
//  ICAOphotoquality
//
//  Copyright © 2017 Marcelo Cobias. All rights reserved.
//
#include "Util.hpp"

bool Util::existsImage(Mat image)
{
    return (!image.empty());
}

Mat Util::cutImage(Mat image, Rect form)
{
  if(existsImage(image))
  {
    Mat imageRoi;
    Mat rect = image.clone();
    imageRoi = rect(form);
    return imageRoi;
  }
  return Mat();
}

Mat Util::resizeImage(Mat image, float percent)
{
  if(existsImage(image))
  {
    Mat dst;
    Size size(image.rows*percent, (image.rows*percent)/0.75);
    resize(image, dst, size);
    return dst;
  }
  return Mat();
}

Mat Util::coloredToGray(Mat image)
{
  if(existsImage(image))
  {
    Mat imageGray;
    Mat imgTemp = image.clone();
    cvtColor(imgTemp, imageGray, CV_BGR2GRAY);
    return imageGray;
  }
  return Mat();
}

float Util::getDistanceBtn2Pnts(Point2f p1, Point2f p2)
{
    float distance = norm(p1 - p2);
    return(distance);
}

/**
* @brief Split a string.
*
* @details Divide uma string usando um caractere.
*
* @param string row string.
*
* @return vector<string> res.
*/
vector<string> Util::splitAtCommas(const string& row)
{
    vector<string> res;
    istringstream buf(row);
    string s;
    while (getline(buf, s, ';'))
        res.push_back(s);
    return res;
}

/**
* @brief Convert int to string
*
* @details Converte inteiro para string
*
* @param int val Integer value
*
* @return string s String value
*/

string Util::intToStr(int val)
{
    string s;
    stringstream out;
    out << val;
    s = out.str();
    return(s);
}

/**
* @brief Convert float to string
*
* @details Converte float para string
*
* @param float val Float value
*
* @return string ss String value
*/
string Util::floToStr(float val)
{
    val = ceil(val * 100) / 100;
    ostringstream ss;
    ss << val;
    return(ss.str());
}

void Util::replaceStringInPlace(string& subject, const string& search, const string& replace)
{
    size_t pos = 0;
    while ((pos = subject.find(search, pos)) != string::npos)
    {
        subject.replace(pos, search.length(), replace);
        pos += replace.length();
    }
}

/**
* @brief Convert string to number
*
* @details Converter String para numero
*
* @param string text String value
*
* @return int ss Number value
*/
float Util::srtToNumber(string &Text)
{
    replaceStringInPlace(Text, ",", ".");
    float result = 0;
    result = atof(Text.c_str());
    return result;
}

/**
* @brief Verify if is a valid image extension.
*
* @details Valida a extens�o da imagem.
*
* @param imageName Complete image file name.
*
* @return res Is a valid image name.
*/
bool Util::isValidImage(string imgName)
{
    bool res = false;

    if (imgName.substr(imgName.find_last_of(".") + 1) == "png" ||
        imgName.substr(imgName.find_last_of(".") + 1) == "jpg" ||
        imgName.substr(imgName.find_last_of(".") + 1) == "PNG" ||
        imgName.substr(imgName.find_last_of(".") + 1) == "JPG" ||
        imgName.substr(imgName.find_last_of(".") + 1) == "ppm" ||
        imgName.substr(imgName.find_last_of(".") + 1) == "PPM") res = true;

    return res;
}

/**
* @brief Split string unging one characterer.
*
* @details Divide uma string.
*
* @param s string.
*
* @return delim characterer to split the string.
*/
vector<string> Util::divStr(string const & s, char delim)
{
    vector<string> result;
    istringstream iss(s);

    for (string token; getline(iss, token, delim); )
    {
        result.push_back(move(token));
    }

    return result;
}

vector<string> Util::getListPathFilesImage(string path)
{
    path += "/*";
    vector<string> types = {".png", ".PNG", ".jpg", ".JPG", ".ppm", ".PPM"};
    glob_t glob_result;
    size_t found;

    vector<string> files;
    glob(path.c_str(),GLOB_TILDE,NULL, &glob_result);

    for(int i=0; i < glob_result.gl_pathc; i++)
    {
        for(int j=0; j < types.size(); j++)
        {
            found = string(glob_result.gl_pathv[i]).find(types[j]);
            if(found != string::npos)
            {
                files.push_back(string(glob_result.gl_pathv[i]));
            }
        }
    }
    globfree(&glob_result);
    return files;
}

bool Util::isDirExist(string path)
{
    struct stat info;
    if (stat(path.c_str(), &info) != 0)
    {
        makePath(path);
        return true;
    }
    return (info.st_mode & S_IFDIR) != 0;
}

bool Util::makePath(string path)
{
    mode_t mode = 0755;
    int ret = mkdir(path.c_str(), mode);
    if (ret == 0)
        return true;

    switch (errno)
    {
        case ENOENT:
        {
            int pos = (int)path.find_last_of('/');
            if (pos == string::npos)
                return false;
            if (!makePath( path.substr(0, pos) ))
                return false;
        }
        return 0 == mkdir(path.c_str(), mode);

        case EEXIST:
            return isDirExist(path);

        default:
            return false;
    }
}

// Finds the intersection of two lines, or returns false.
// The lines are defined by (o1, p1) and (o2, p2).
bool Util::intersecPoints(Point2f o1, Point2f p1, Point2f o2, Point2f p2, Point2f &r)
{
    Point2f x = o2 - o1;
    Point2f d1 = p1 - o1;
    Point2f d2 = p2 - o2;

    float cross = d1.x*d2.y - d1.y*d2.x;
    if (abs(cross) < /*EPS*/1e-8)
        return false;

    double t1 = (x.x * d2.y - x.y * d2.x)/cross;
    r = o1 + d1 * t1;
    return true;
}

void Util::glcm(Mat &img,  vector<float> &values)
{
    values = vector<float>(6);
    int row=img.rows,col=img.cols;
    Mat gl=Mat::zeros(256,256,CV_32FC1);

    //creating glcm matrix with 256 levels,radius=1 and in the horizontal direction
    for(int i=0;i<row;i++)
        for(int j=0;j<col-1;j++)
            gl.at<float>(img.at<uchar>(i,j),img.at<uchar>(i,j+1))=gl.at<float>(img.at<uchar>(i,j),img.at<uchar>(i,j+1))+1;

    // normalizing glcm matrix for parameter determination
    gl=gl+gl.t();
    gl=gl/sum(gl)[0];


    for(int i=0;i<256;i++)
    {
        for(int j=0;j<256;j++)
        {
            values[0]=values[0]+gl.at<float>(i,j)*gl.at<float>(i,j);            //finding parameters
            values[1]=values[1]+(i-j)*(i-j)*gl.at<float>(i,j);
            values[2]=values[2]+gl.at<float>(i,j)/(1+abs(i-j));
            if(i!=j)
                values[3]=values[3]+gl.at<float>(i,j)/((i-j)*(i-j));                      //Taking k=2;
            if(gl.at<float>(i,j)!=0)
                values[4]=values[4]-gl.at<float>(i,j)*log10(gl.at<float>(i,j));
            values[5]=values[5]+0.5*(i*gl.at<float>(i,j)+j*gl.at<float>(i,j));
        }
    }
}

void Util::showHistogram(Mat& img)
{
    int bins = img.cols;             // number of bins
    int nc = img.channels();    // number of channels
    vector<Mat> hist(nc);       // array for storing the histograms
    vector<Mat> canvas(nc);     // images for displaying the histogram
    int hmax[3] = {0,0,0};      // peak value for each histogram

    // The rest of the code will be placed here
    for (int i = 0; i < hist.size(); i++)
        hist[i] = Mat::zeros(1, bins, CV_32SC1);
    for (int i = 0; i < img.rows; i++)
    {
        for (int j = 0; j < img.cols; j++)
        {
            for (int k = 0; k < nc; k++)
            {
                uchar val = nc == 1 ? img.at<uchar>(i,j) : img.at<Vec3b>(i,j)[k];
                hist[k].at<int>(val) += 1;
            }
        }
    }
    for (int i = 0; i < nc; i++)
    {
        for (int j = 0; j < bins-1; j++)
            hmax[i] = hist[i].at<int>(j) > hmax[i] ? hist[i].at<int>(j) : hmax[i];
    }
    const char* wname[3] = { "blue", "green", "red" };
    Scalar colors[3] = { Scalar(255,0,0), Scalar(0,255,0), Scalar(0,0,255) };

    for (int i = 0; i < nc; i++)
    {
        canvas[i] = Mat::ones(125, bins, CV_8UC3);

        for (int j = 0, rows = canvas[i].rows; j < bins-1; j++)
        {
            line(
                 canvas[i],
                 Point2f(j, rows),
                 Point2f(j, rows - (hist[i].at<int>(j) * rows/hmax[i])),
                 nc == 1 ? Scalar(200,200,200) : colors[i],
                 1, 8, 0
                 );
        }

        imshow(nc == 1 ? "value" : wname[i], canvas[i]);
        //imwrite("histp.jpg",canvas[i]);

    }
}

Vec3b Util::getColorSubpix(const Mat& img, Point2f pt)
{
    assert(!img.empty());
    assert(img.channels() == 3);

    int x = (int)pt.x;
    int y = (int)pt.y;

    int x0 = borderInterpolate(x,   img.cols, BORDER_REFLECT_101);
    int x1 = borderInterpolate(x+1, img.cols, BORDER_REFLECT_101);
    int y0 = borderInterpolate(y,   img.rows, BORDER_REFLECT_101);
    int y1 = borderInterpolate(y+1, img.rows, BORDER_REFLECT_101);

    float a = pt.x - (float)x;
    float c = pt.y - (float)y;

    uchar b = (uchar)cvRound((img.at<Vec3b>(y0, x0)[0] * (1.f - a) + img.at<Vec3b>(y0, x1)[0] * a) * (1.f - c)
                             + (img.at<Vec3b>(y1, x0)[0] * (1.f - a) + img.at<Vec3b>(y1, x1)[0] * a) * c);
    uchar g = (uchar)cvRound((img.at<Vec3b>(y0, x0)[1] * (1.f - a) + img.at<Vec3b>(y0, x1)[1] * a) * (1.f - c)
                             + (img.at<Vec3b>(y1, x0)[1] * (1.f - a) + img.at<Vec3b>(y1, x1)[1] * a) * c);
    uchar r = (uchar)cvRound((img.at<Vec3b>(y0, x0)[2] * (1.f - a) + img.at<Vec3b>(y0, x1)[2] * a) * (1.f - c)
                             + (img.at<Vec3b>(y1, x0)[2] * (1.f - a) + img.at<Vec3b>(y1, x1)[2] * a) * c);

    return Vec3b(b, g, r);
}
