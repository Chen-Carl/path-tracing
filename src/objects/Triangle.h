#ifndef __OBJECTS_TRIANGLE_H__
#define __OBJECTS_TRIANGLE_H__

#include "objects/Object.h"

class Triangle : public Object
{
private:
    std::array<cv::Vec3f, 3> m_vertices;
    // the normal of the triangle
    std::array<cv::Vec3f, 3> m_normal;
    // the texture coordinates of the triangle
    std::array<cv::Vec2f, 3> m_texCoords;

public:
    Triangle();
    Triangle(const std::array<cv::Vec3f, 3> &vertices);

    virtual std::optional<HitPayload> intersect(const cv::Vec3f &orig, const cv::Vec3f &dir) const override;

    virtual AABB getAABB() const override;

    virtual cv::Vec3f getNormal(const cv::Vec3f &point) const override;

    // set i-th vertex coordinate
    void setVertex(int index, const cv::Vec3f &vertex) { m_vertices[index] = vertex; }
    // set i-th vertex normal vector
    void setNormal(int index, const cv::Vec3f &normal) { m_normal[index] = normal; }
    // set i-th vertex texture coordinate
    void setTexCoord(int index, const cv::Vec2f &texCoord) { m_texCoords[index] = texCoord; }
};

#endif