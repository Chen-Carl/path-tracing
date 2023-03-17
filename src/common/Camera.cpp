#include "common/Camera.h"

Camera::Camera(int width_, int height_, float fov_, 
    const cv::Vec3f &eyePos_, 
    const cv::Vec3f &lookat_, 
    const cv::Vec3f &up_
 ) : width(width_), height(height_), fov(fov_),
    eyePos(eyePos_), lookat(lookat_), up(up_)
{ 
    init();
}

void Camera::init()
{
    float scale = std::tan(zoe::deg2rad(fov) / 2.0f);
    float imgAspectRatio = width / static_cast<float>(height);

    w = cv::normalize(lookat - eyePos);
    const cv::Vec3f u = cv::normalize(up.cross(w));
    const cv::Vec3f v = w.cross(u);

    horizontal = 2.0f * scale * imgAspectRatio * u;
    vertical = 2.0f * scale * v;
}


cv::Vec3f Camera::getRayDir(int x, int y) const
{
    return w + horizontal / 2.0f + vertical / 2.0f 
        - (x + 0.5f) / width * horizontal
        - (y + 0.5f) / height * vertical;
}