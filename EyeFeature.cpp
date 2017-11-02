//
//  ICAOphotoquality
//
//  Copyright © 2017 Marcelo Cobias. All rights reserved.
//
#include "EyeFeature.hpp"

/*void identifiereyes::eyesOpen(cv::Mat image, std::vector<cv::Point2f> landmarks, cv::Rect rect_face)
{
    //cv::Rect eyes_pair(rect_face.x, rect_face.y, rect_face.width, (landmarks[1].y - rect_face.y));
    //cv::Mat img_eye_pair = imagetester::applyRoi(image, eyes_pair);
    std::vector<float> dist_points_eye_right(3);
    std::vector<float> dist_points_eye_left(3);

    dist_center_eyes = cv::norm(center_iris_right - center_iris_left);

    dist_points_eye_right[0] = cv::norm(landmarks_points_eyes_right_eyelid[2] - landmarks_points_eyes_right_eyelid[10]);
    dist_points_eye_right[1] = cv::norm(landmarks_points_eyes_right_eyelid[3] - landmarks_points_eyes_right_eyelid[9]);
    dist_points_eye_right[2] = cv::norm(landmarks_points_eyes_right_eyelid[4] - landmarks_points_eyes_right_eyelid[8]);

    dist_points_eye_left[0] = cv::norm(landmarks_points_eyes_left_eyelid[2] - landmarks_points_eyes_left_eyelid[10]);
    dist_points_eye_left[1] = cv::norm(landmarks_points_eyes_left_eyelid[3] - landmarks_points_eyes_left_eyelid[9]);
    dist_points_eye_left[2] = cv::norm(landmarks_points_eyes_left_eyelid[4] - landmarks_points_eyes_left_eyelid[8]);

    for(int i = 0; i < dist_points_eye_right.size(); i++)
    {
        dist_open_eye_right_eyelid += dist_points_eye_right[i];
        dist_open_eye_left_eyelid += dist_points_eye_left[i];
    }
    dist_open_eye_right_eyelid/= dist_points_eye_right.size();
    dist_open_eye_left_eyelid/= dist_points_eye_left.size();
}

void identifiereyes::eyesGlasses(cv::Mat image, std::vector<cv::Point2f> landmarks, cv::Rect rect_face)
{
    std::vector<float> values;
    std::vector<float> value_glasses;

    cv::Point2f glasses_point_right;
    identifierutils::intersection(cv::Point2f(0, landmarks[27].y), landmarks[27], cv::Point2f(landmarks[0].x, 0), landmarks[0], glasses_point_right);
    glasses_point_right += landmarks[17];
    glasses_point_right/=2;
    for(float i = 1.; i < 1.05; i+=0.01)
    {
        cv::Rect area_glasses_right(glasses_point_right.x, glasses_point_right.y * i, 20, 10);
        cv::Mat image_glasses_right = identifierimage::applyRoi(image, area_glasses_right);
        identifierutils::glcm(image_glasses_right, values);
        value_glasses.push_back(values[1]);
        //cv::imshow("teste_rigth", image_glasses_right);
    }
    std::sort(value_glasses.begin(), value_glasses.end());
    right_eye_contrast_glcm = (value_glasses[3] + value_glasses[4])/2;

    cv::Point2f glasses_point_left;
    identifierutils::intersection(cv::Point2f(0, landmarks[27].y), landmarks[27], cv::Point2f(landmarks[16].x, 0), landmarks[16], glasses_point_left);
    glasses_point_left += landmarks[26];
    glasses_point_left/=2;

    value_glasses.clear();
    for(float i = 1.; i < 1.05; i+=0.01)
    {
        cv::Rect area_glasses_left(glasses_point_right.x, glasses_point_left.y * i, 20, 10);
        cv::Mat image_glasses_left = identifierimage::applyRoi(image, area_glasses_left);
        identifierutils::glcm(image_glasses_left, values);
        value_glasses.push_back(values[1]);
        //cv::imshow("teste_left", image_glasses_left);
    }
    std::sort(value_glasses.begin(), value_glasses.end());
    left_eye_contrast_glcm = (value_glasses[3] + value_glasses[4])/2;

    cv::Rect area_nose((landmarks[39].x + landmarks[27].x)/2, landmarks[29].y, 20, 10);
    cv::Mat image_nose = identifierimage::applyRoi(image, area_nose);
    identifierutils::glcm(image_nose, values);
    center_nose_contrast_glcm = values[1];

    value_glasses.clear();
    cv::Point2f glasses_center_left = landmarks[27];
    for(float i = 1.01; i < 1.06; i+=0.01)
    {
        cv::Rect area_glasses_center(glasses_center_left.x - ((landmarks[42].x - landmarks[39].x)*.2), ((landmarks[27].y + landmarks[21].y)/2)*i, (landmarks[42].x - landmarks[39].x)*.4, 10);
        cv::Mat image_glasses_center = identifierimage::applyRoi(image, area_glasses_center);
        identifierutils::glcm(image_glasses_center, values);
        value_glasses.push_back(values[1]);
        //cv::imshow("teste_center", image_glasses_center);
    }
    std::sort(value_glasses.begin(), value_glasses.end());
    center_eye_contrast_glcm = (value_glasses[3] + value_glasses[4])/2;

    //cv::imshow("teste_nose", image_nose);
    //cv::circle(image, glasses_point_right, 2, cv::Scalar(0, 0, 255));
    //cv::circle(image, glasses_point_left, 2, cv::Scalar(0, 0, 255));
    //cv::imshow("teste_face", imagetester::applyRoi(image, rect_face));
    //cv::waitKey(100);
}

void identifiereyes::eyesLookingAway(cv::Mat image, std::vector<cv::Point2f> landmarks)
{
    center_estimate_eyes_right = landmarks_points_eyes_right_eyelid[0];
    center_estimate_eyes_right += landmarks_points_eyes_right_eyelid[6];
    center_estimate_eyes_right += landmarks_points_eyes_right_eyelid[3];
    center_estimate_eyes_right += landmarks_points_eyes_right_eyelid[9];
    center_estimate_eyes_right /= 4;

    center_estimate_eyes_left = landmarks_points_eyes_left_eyelid[0];
    center_estimate_eyes_left += landmarks_points_eyes_left_eyelid[6];
    center_estimate_eyes_left += landmarks_points_eyes_left_eyelid[3];
    center_estimate_eyes_left += landmarks_points_eyes_left_eyelid[9];
    center_estimate_eyes_left /= 4;

    look_eye_right_horizontal = std::abs(center_estimate_eyes_right.x - center_iris_right.x);
    look_eye_right_vertical = std::abs(center_estimate_eyes_right.y - center_iris_right.y);

    look_eye_left_horizontal = std::abs(center_estimate_eyes_left.x - center_iris_left.x);
    look_eye_left_vertical = std::abs(center_estimate_eyes_left.y - center_iris_left.y);
}*/
