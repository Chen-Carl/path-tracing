#ifndef __COMMON_RAY_H__
#define __COMMON_RAY_H__

#include <opencv2/opencv.hpp>

class Ray
{
private:
    cv::Vec3f m_orig;
    cv::Vec3f m_dir;

public:
    Ray() = delete;
    Ray(const cv::Vec3f &orig, const cv::Vec3f &dir) :
        m_orig(orig),
        m_dir(dir)
    {
        // m_dir = cv::normalize(m_dir);
    }

    const cv::Vec3f &getOrig() const { return m_orig; }
    const cv::Vec3f &getDir() const { return m_dir; }
};

#endif