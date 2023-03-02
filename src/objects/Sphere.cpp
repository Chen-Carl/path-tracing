#include "common/utils.h"
#include "objects/Sphere.h"

Sphere::Sphere(const cv::Vec3f &center, float radius)
{
    m_center = center;
    m_radius = radius;
}

std::optional<HitPayload> Sphere::intersect(const cv::Vec3f &orig, const cv::Vec3f &dir) const
{
    float a = dir.dot(dir);
    float b = 2 * dir.dot(orig - m_center);
    float c = (orig - m_center).dot(orig - m_center) - m_radius * m_radius;
    auto x = zoe::solveQuad(a, b, c);
    if (!x.has_value())
    {
        return std::nullopt;
    }
    auto [x1, x2] = x.value();
    if (x1 < 0)
    {
        if (x2 < 0)
        {
            return std::nullopt;
        }
        return HitPayload(cv::Vec2f(0.0, 0.0), shared_from_this(), x2);
    }
    return HitPayload(cv::Vec2f(0.0, 0.0), shared_from_this(), x1);
}

AABB Sphere::getAABB() const
{
    return AABB(
        m_center - cv::Vec3f(m_radius, m_radius, m_radius), 
        m_center + cv::Vec3f(m_radius, m_radius, m_radius)
    );
}


cv::Vec3f Sphere::getNormal(const cv::Vec3f &point) const
{
    return cv::normalize(point - m_center);
}
