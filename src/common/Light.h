#ifndef __LIGHT_H__
#define __LIGHT_H__

#include <opencv2/opencv.hpp>

class Light
{
private:
    cv::Vec3f m_pos;
    cv::Vec3f m_intensity;

public:
    Light();
    Light(cv::Vec3f pos, cv::Vec3f intensity);
    virtual ~Light() = default;

    const cv::Vec3f &getPos() const { return m_pos; }
    const cv::Vec3f &getIntensity() const { return m_intensity; }
};

#endif