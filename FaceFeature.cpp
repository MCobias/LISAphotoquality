//
//  ICAOphotoquality
//
//  Copyright © 2017 Marcelo Cobias. All rights reserved.
//
#include "FaceFeature.hpp"

/*void identifierface::faceCentralized(cv::Mat image, vector<cv::Point2f> landmarks)
{
    cv::Point2f point_image_center(0, 0);
    point_image_center =  landmarks[0];
    point_image_center += landmarks[16];
    point_image_center/=2;

    cv::Point2f point_image_top(point_image_center.x, 0);
    cv::Point2f point_image_medium(0, point_image_center.y);

    float dist_width = cv::norm(point_image_center - point_image_medium);
    float dist_height = cv::norm(point_image_center - point_image_top);

    horizontal_percent_head = (dist_width * 100) / image.cols;
    vertical_percent_head = (dist_height * 100) / image.rows;
}

void identifierface::faceProportion(cv::Mat image, vector<cv::Point2f> landmarks)
{
    float head_width = cv::norm(landmarks[0] - landmarks[16]); //left ear - right ear

    cv::Mat image_canny = image.clone();
    cv::Mat detected_edges;

    cv::blur(image_canny, detected_edges, cv::Size(3,3));
    cv::Canny(detected_edges, detected_edges, 10, 200, 3);

    for (int i = 0; i < 3; i++)
        cv::dilate(detected_edges, detected_edges, cv::Mat());

    //imshow("Canny", detected_edges);

    int j;
    for(j = 0; !detected_edges.at<uchar>(j, landmarks[8].x); j++)
        detected_edges.at<uchar>(j, landmarks[8].x) = 255;

    head_point_top = cv::Point(landmarks[8].x, j + 5);

    float head_heigth = cv::norm(head_point_top - landmarks[8]);

    head_area_total = ((head_width * head_heigth) * 100) / image.size().area();
}

void identifierface::headEstimatePosition(cv::Mat image, vector<cv::Point2f> &landmarks, cv::Point2f center_eye_right, cv::Point2f center_eye_left)
{
    std::vector<double> rv(3), tv(3);
    cv::Mat rvec(rv),tvec(tv);
    cv::Vec3d eav;

    std::vector<cv::Point3f> model_points;

    // Labelling the 3D Points derived from a 3D model of human face.
    // You may replace these points as per your custom 3D head model if any
    model_points.push_back(cv::Point3f(2.37427, 110.322, 21.7776));	// l eye (v 314)
    model_points.push_back(cv::Point3f(70.0602, 109.898, 20.8234));	// r eye (v 0)
    model_points.push_back(cv::Point3f(36.8301, 78.3185, 52.0345));	//nose (v 1879)
    model_points.push_back(cv::Point3f(14.8498, 51.0115, 30.2378));	// l mouth (v 1502)
    model_points.push_back(cv::Point3f(58.1825, 51.0115, 29.6224));	// r mouth (v 695)
    model_points.push_back(cv::Point3f(-61.8886, 127.797, -89.4523));	// l ear (v 2011)
    model_points.push_back(cv::Point3f(127.603, 126.9, -83.9129));		// r ear (v 1138)

    std::vector<cv::Point2f> image_points;
    image_points.push_back(center_eye_left); // left eye
    image_points.push_back(center_eye_right); // right eye
    image_points.push_back(landmarks[30]); // nose
    image_points.push_back(landmarks[48]); //left lip corner
    image_points.push_back(landmarks[54]); // right lip corner
    image_points.push_back(landmarks[0]); // left ear
    image_points.push_back(landmarks[16]); // right ear

    cv::Mat ip(image_points);
    cv::Mat op = cv::Mat(model_points);
    rvec = cv::Mat(rv);

    double _d[9] = {1, 0, 0 , 0, -1, 0, 0, 0, -1};
    Rodrigues(cv::Mat(3,3,CV_64FC1,_d),rvec);
    tv[0]=0;tv[1]=0;tv[2]=1;
    tvec = cv::Mat(tv);

    double max_d = MAX(image.rows, image.cols);
    double _cm[9] = {max_d,     0, (double)image.cols/2.0, 0, max_d, (double)image.rows/2.0, 0, 0, 1.0};
    cv::Mat camMatrix = cv::Mat(3,3,CV_64FC1, _cm);

    double _dc[] = {0,0,0,0};
    solvePnPRansac(op,ip,camMatrix,cv::Mat(1,4,CV_64FC1,_dc),rvec,tvec,false,CV_EPNP);

    double rot[9] = {0};
    cv::Mat rotM(3,3,CV_64FC1,rot);
    Rodrigues(rvec,rotM);
    double* _r = rotM.ptr<double>();

    double _pm[12] = {_r[0],_r[1],_r[2],tv[0],
        _r[3],_r[4],_r[5],tv[1],
        _r[6],_r[7],_r[8],tv[2]};

    cv::Mat tmp,tmp1,tmp2,tmp3,tmp4,tmp5;
    cv::decomposeProjectionMatrix(cv::Mat(3, 4, CV_64FC1, _pm),tmp,tmp1,tmp2,tmp3,tmp4,tmp5,eav);

    pose_head_estimate[0] = eav[1];
    pose_head_estimate[1] = eav[0];
    pose_head_estimate[2] = eav[2];
}*/
