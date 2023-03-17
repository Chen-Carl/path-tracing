#ifndef __COMMON_CAMERA_H__
#define __COMMON_CAMERA_H__

#include <opencv2/opencv.hpp>
#include "common/utils.h"

class Camera
{
private:
    cv::Vec3f horizontal;
    cv::Vec3f vertical;
    cv::Vec3f w;

public:
    int width = 1280;
    int height = 960;
    float fov = 90.0f;
    cv::Vec3f eyePos = {0, 0, 0};
    cv::Vec3f lookat = {0, 0, -1};
    cv::Vec3f up = {0, 1, 0};

    Camera() { }

    Camera(int width_, int height_, float fov_, 
        const cv::Vec3f &eyePos_ = cv::Vec3f(0, 0, 0), 
        const cv::Vec3f &lookat_ = cv::Vec3f(0, 0, -1), 
        const cv::Vec3f &up_ = cv::Vec3f(0, 1, 0));

    cv::Vec3f getRayDir(int x, int y) const;

    void init();
};

#endif