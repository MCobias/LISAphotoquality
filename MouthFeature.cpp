//
//  ICAOphotoquality
//
//  Copyright © 2017 Marcelo Cobias. All rights reserved.
//
#include "MouthFeature.hpp"

/*void identifiermouth::mothBeardOrObstruction(cv::Mat image, std::vector<cv::Point2f> landmarks, cv::Rect rect_face)
{
    cv::Mat image_canny = image.clone();
    cv::Mat detected_edges;

    cv::blur(image_canny, detected_edges, cv::Size(3, 3));
    cv::Canny(detected_edges, detected_edges, 0, 150, 3);
    cv::dilate(detected_edges, detected_edges, cv::Mat());

    cv::Rect rect_beard(landmarks[3].x, landmarks[33].y, (landmarks[13].x - landmarks[3].x),  (landmarks[9].y - landmarks[33].y));
    detected_edges = identifierimage::applyRoi(detected_edges, rect_beard);

    float total_pixels = detected_edges.rows * detected_edges.cols;
    float black_pixels = total_pixels - cv::countNonZero(detected_edges);
    float white_pixels = total_pixels - black_pixels;

    qtd_percent_beard = (white_pixels * 100)/total_pixels;

    cv::Rect area_left_beard(landmarks[4].x, landmarks[4].y, 50, 50);
    cv::Mat image_left_beard = identifierimage::applyRoi(image, area_left_beard);
    cvtColor(image_left_beard, image_left_beard, CV_BGR2GRAY);

    cv::Rect area_right_beard(landmarks[13].x * 0.8, landmarks[13].y, 50, 50);
    cv::Mat image_right_beard = identifierimage::applyRoi(image, area_right_beard);
    cvtColor(image_right_beard, image_right_beard, CV_BGR2GRAY);

    cv::Rect area_center_beard(landmarks[7].x, landmarks[7].y * 0.9, 50, 50);
    cv::Mat image_center_beard = identifierimage::applyRoi(image, area_center_beard);
    cvtColor(image_center_beard, image_center_beard, CV_BGR2GRAY);

    cv::Point2f point_mustache((landmarks[31].x + landmarks[3].x)/2, (landmarks[31].y + landmarks[3].y)/2);
    cv::Rect area_mustache(point_mustache.x, point_mustache.y, landmarks[45].x - landmarks[36].x, (landmarks[4].y - landmarks[33].y)/2);
    cv::Mat image_mustache = identifierimage::applyRoi(image, area_mustache);
    cvtColor(image_mustache, image_mustache, CV_BGR2GRAY);

    std::vector<float> values;
    identifierutils::glcm(image_left_beard, values);
    left_beard_contrast_glcm = values[1];
    identifierutils::glcm(image_right_beard, values);
    right_beard_contrast_glcm = values[1];
    identifierutils::glcm(image_center_beard, values);
    center_beard_contrast_glcm = values[1];
    identifierutils::glcm(image_mustache, values);
    mustache_contrast_glcm = values[1];

    imshow("Canny", image_mustache);
    cv::waitKey(100);
}
void identifiermouth::mothOpen()
{
    std::vector<float> dist_points_lips(3);
    dist_points_lips[0] = cv::norm(landmarks_lips[13] - landmarks_lips[19]);
    dist_points_lips[1] = cv::norm(landmarks_lips[14] - landmarks_lips[18]);
    dist_points_lips[2] = cv::norm(landmarks_lips[15] - landmarks_lips[17]);

    for(int i = 0; i < dist_points_lips.size(); i++)
    {
        dist_top_down_lisps += dist_points_lips[i];
    }
    dist_top_down_lisps /= dist_points_lips.size();
}*/
