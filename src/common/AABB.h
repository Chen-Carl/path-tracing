#ifndef __COMMON_AABB_H__
#define __COMMON_AABB_H__

#include <opencv2/opencv.hpp>
#include "common/Ray.h"

class AABB
{
private:
    cv::Vec3f m_min;
    cv::Vec3f m_max;

public:
    AABB();
    AABB(const cv::Vec3f &min, const cv::Vec3f &max);

    cv::Vec3f getCentroid() const { return (m_min + m_max) / 2; }
    cv::Vec3f getDiagonal() const { return m_max - m_min; }
    int getLargestAxis() const;

    bool intersect(const Ray &ray) const;

    AABB operator+(const AABB &other) const;
    AABB operator+(const cv::Vec3f &vec) const;

    friend AABB operator+(const cv::Vec3f &vec, const AABB &aabb);
    friend std::ostream &operator<<(std::ostream &os, const AABB &aabb);
};

#endif