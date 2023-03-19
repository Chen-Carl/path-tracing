#ifndef __OBJECTS_TRIANGLE_H__
#define __OBJECTS_TRIANGLE_H__

#include "objects/Object.h"

class Triangle : public Object
{
private:
    std::array<cv::Vec3f, 3> m_vertices;
    // the vertex normal of the triangle
    std::array<cv::Vec3f, 3> m_vNormal;
    // the texture coordinates of the triangle
    std::array<cv::Vec2f, 3> m_texCoords;
    // the vertex color of the triangle
    std::array<cv::Vec3f, 3> m_vColor;
    // the normal of the triangle
    cv::Vec3f m_normal;

public:
    Triangle();
    Triangle(const std::array<cv::Vec3f, 3> &vertices);

    virtual std::optional<HitPayload> intersect(const Ray &ray) const override;

    virtual AABB getAABB() const override;
    virtual cv::Vec3f getNormal(const cv::Vec3f &point) const override;
    virtual cv::Vec3f getDiffuseColor(const cv::Vec2f &uv) const override;
    virtual float getArea() const override;
    virtual HitPayload samplePoint() const override;

    virtual cv::Vec2f getTexCoords(const cv::Vec2f &uv) const override;

    // set i-th vertex coordinate
    void setVertex(int index, const cv::Vec3f &vertex) { m_vertices[index] = vertex; }
    // set i-th vertex normal vector
    void setNormal(int index, const cv::Vec3f &normal) { m_vNormal[index] = normal; }
    // set i-th vertex texture coordinate
    void setNormals(const std::array<cv::Vec3f, 3> &normals) { m_vNormal = normals; }
    void setTexCoord(int index, const cv::Vec2f &texCoord) { m_texCoords[index] = texCoord; }
    void setTexCoords(const std::array<cv::Vec2f, 3> &texCoords) { m_texCoords = texCoords; }
    void setColors(const std::array<cv::Vec3f, 3> &colors) { m_vColor = colors; }

    static std::optional<std::vector<Triangle>> loadModel(const std::string &filepath);
};

#endif