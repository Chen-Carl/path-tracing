#ifndef __COMMON_CAMERA_H__
#define __COMMON_CAMERA_H__

#include <opencv2/opencv.hpp>

struct Camera
{
    cv::Vec3f bgColor = {0, 0, 0};
    cv::Vec3f eyePos = {0, 0, 0};
    float fov = 90.0f;
};

#endif