#ifndef __COMMON_CAMERA_H__
#define __COMMON_CAMERA_H__

#include <opencv2/opencv.hpp>

struct Camera
{
    int width = 1280;
    int height = 960;
    float fov = 90.0f;
    cv::Vec3f eyePos = {0, 0, 0};
    cv::Vec3f lookat = {0, 0, 0};
    cv::Vec3f up = {0, 0, 0};

    Camera() { }

    Camera(int width, int height, float fov) : width(width), height(height), fov(fov) { }
};

#endif