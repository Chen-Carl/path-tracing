#include "objects/Triangle.h"
#include "Triangle.h"

Triangle::Triangle()
{
    m_vertices = std::array<cv::Vec3f, 3>();
}

Triangle::Triangle(const std::array<cv::Vec3f, 3> &vertices) :
    m_vertices(vertices)
{

}

std::optional<HitPayload> Triangle::intersect(const cv::Vec3f &orig, const cv::Vec3f &dir) const
{
    cv::Vec3f edge1 = m_vertices[1] - m_vertices[0];
    cv::Vec3f edge2 = m_vertices[2] - m_vertices[0];
    cv::Vec3f s = orig - m_vertices[0];
    cv::Vec3f s1 = dir.cross(edge2);
    cv::Vec3f s2 = s.cross(edge1);

    float tmp = 1.0 / s1.dot(edge1);
    float t = tmp * s2.dot(edge2);
    float u = tmp * s1.dot(s);
    float v = tmp * s2.dot(dir);

    if (t < 0 || u < 0 || v < 0 || u + v > 1)
    {
        return std::nullopt;
    }

    return HitPayload(cv::Vec2f(u, v), shared_from_this(), t);
}

AABB Triangle::getAABB() const
{
    cv::Vec3f min = m_vertices[0];
    cv::Vec3f max = m_vertices[0];
    for (int i = 1; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            min[j] = std::min(min[j], m_vertices[i][j]);
            max[j] = std::max(max[j], m_vertices[i][j]);
        }
    }
    return AABB(min, max);
}

cv::Vec3f Triangle::getNormal(const cv::Vec3f &point) const
{
    cv::Vec3f v0 = m_vertices[1] - m_vertices[0];
    cv::Vec3f v1 = m_vertices[2] - m_vertices[0];
    cv::Vec3f normal = v0.cross(v1);
    normal = normal / cv::norm(normal);
    return normal;
}
