#include "AABB.h"

AABB::AABB()
{
    float min = std::numeric_limits<float>::lowest();
    float max = std::numeric_limits<float>::max();
    m_min = cv::Vec3f(max, max, max);
    m_max = cv::Vec3f(min, min, min);
}

AABB::AABB(const cv::Vec3f &min, const cv::Vec3f &max) :
    m_min(min),
    m_max(max)
{

}

int AABB::getLargestAxis() const
{
    cv::Vec3f diag = getDiagonal();
    if (diag[0] > diag[1] && diag[0] > diag[2])
        return 0;
    else if (diag[1] > diag[2])
        return 1;
    else
        return 2;
}

bool AABB::intersect(const Ray &ray) const
{
    const cv::Vec3f &orig = ray.getOrig();
    const cv::Vec3f &dir = ray.getDir();
    float enter = std::numeric_limits<float>::lowest();
    float exit = std::numeric_limits<float>::max();
    std::vector<bool> neg {
        dir[0] > 0,
        dir[1] > 0,
        dir[2] > 0
    };

    for (int i = 0; i < 3; i++)
    {
        float t1 = (m_min[i] - orig[i]) / dir[i];
        float t2 = (m_max[i] - orig[i]) / dir[i];
        enter = std::max(enter, neg[i] ? t1 : t2);
        exit = std::min(exit, neg[i] ? t2 : t1);
    }
    return enter < exit && exit >= 0;
}

AABB AABB::operator+(const AABB &other) const
{
    cv::Vec3f min = cv::Vec3f(
        std::min(m_min[0], other.m_min[0]),
        std::min(m_min[1], other.m_min[1]),
        std::min(m_min[2], other.m_min[2])
    );
    cv::Vec3f max = cv::Vec3f(
        std::max(m_max[0], other.m_max[0]),
        std::max(m_max[1], other.m_max[1]),
        std::max(m_max[2], other.m_max[2])
    );
    return AABB(min, max);
}

AABB AABB::operator+(const cv::Vec3f &vec) const
{
    cv::Vec3f min = cv::Vec3f(
        std::min(m_min[0], vec[0]),
        std::min(m_min[1], vec[1]),
        std::min(m_min[2], vec[2])
    );
    cv::Vec3f max = cv::Vec3f(
        std::max(m_max[0], vec[0]),
        std::max(m_max[1], vec[1]),
        std::max(m_max[2], vec[2])
    );
    return AABB(min, max);
}

AABB operator+(const cv::Vec3f &vec, const AABB &aabb)
{
    return aabb + vec;
}

std::ostream &operator<<(std::ostream &os, const AABB &aabb)
{
    os << "AABB(" << aabb.m_min << ", " << aabb.m_max << ")";
    return os;
}
