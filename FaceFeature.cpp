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
*/
