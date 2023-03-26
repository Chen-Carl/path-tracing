#include <cmath>
#include "objects/Triangle.h"
#include "common/OBJ_Loader.h"
#include "common/utils.h"
#include "Triangle.h"

Triangle::Triangle()
{
    m_vertices = std::array<cv::Vec3f, 3>();
}

Triangle::Triangle(const std::array<cv::Vec3f, 3> &vertices) :
    m_vertices(vertices)
{
    cv::Vec3f edge1 = m_vertices[1] - m_vertices[0];
    cv::Vec3f edge2 = m_vertices[2] - m_vertices[0];
    m_normal = cv::normalize(edge1.cross(edge2));
}

std::optional<HitPayload> Triangle::intersect(const Ray &ray) const
{
    const cv::Vec3f &orig = ray.getOrig();
    const cv::Vec3f &dir = ray.getDir();
    cv::Vec3f edge1 = m_vertices[1] - m_vertices[0];
    cv::Vec3f edge2 = m_vertices[2] - m_vertices[0];
    cv::Vec3f s = orig - m_vertices[0];
    cv::Vec3f s1 = dir.cross(edge2);
    cv::Vec3f s2 = s.cross(edge1);

    float tmp = 1.0 / (s1.dot(edge1) == 0 ? zoe::denominatorEpsilon : s1.dot(edge1));
    float t = tmp * s2.dot(edge2);
    float u = tmp * s1.dot(s);
    float v = tmp * s2.dot(dir);

    if (t < zoe::selfCrossEpsilon || u < 0 || v < 0 || u + v > 1)
    {
        return std::nullopt;
    }

    HitPayload res(cv::Vec2f(u, v), shared_from_this(), t, getEmission());
    res.point = orig + t * dir;
    return res;
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
    if (m_normal != cv::Vec3f(0, 0, 0))
    {
        return m_normal;
    }
    cv::Vec3f v0 = m_vertices[1] - m_vertices[0];
    cv::Vec3f v1 = m_vertices[2] - m_vertices[0];
    cv::Vec3f normal = v0.cross(v1);
    normal = normal / cv::norm(normal);
    return normal;
}

cv::Vec3f Triangle::getDiffuseColor(const cv::Vec2f &uv) const
{
    // cv::Vec3f color = Object::getDiffuseColor(uv);
    // if (color != cv::Vec3f(0, 0, 0))
    // {
    //     return color;
    // }
    
    // cv::Vec3f color1(0.031, 0.235, 0.815);
    // cv::Vec3f color2(0.231, 0.937, 0.937);
    // float scale = 5;
    // float pattern = (std::fmod(uv[0] * scale, 1) > 0.5) ^ (std::fmod(uv[1] * scale, 1) > 0.5);
    // return (1 - pattern) * color1 + pattern * color2;

    if (getTexture() != nullptr)
    {
        cv::Vec2f st = getTexCoords(uv);
        int i = static_cast<int>(st[0] * getTexture()->rows);
        int j = static_cast<int>(st[1] * getTexture()->cols);
        return getTexture()->at<cv::Vec3f>(i, j) / 255;
    }

    return cv::Vec3f(1, 1, 1);
}

float Triangle::getArea() const
{
    return 0.5 * cv::norm((m_vertices[1] - m_vertices[0]).cross(m_vertices[2] - m_vertices[0]));
}

HitPayload Triangle::samplePoint() const
{
    float x = std::sqrt(zoe::randomFloat());
    float y = zoe::randomFloat();
    cv::Vec3f point = (1 - x) * m_vertices[0] 
            + x * (1 - y) * m_vertices[1] 
            + x * y * m_vertices[2];

    HitPayload payload;
    payload.point = point;
    payload.hitObj = shared_from_this();
    payload.emission = getEmission();
    return payload;
}

cv::Vec2f Triangle::getTexCoords(const cv::Vec2f &uv) const
{
    const cv::Vec2f &st0 = m_texCoords[0];
    const cv::Vec2f &st1 = m_texCoords[1];
    const cv::Vec2f &st2 = m_texCoords[2];
    cv::Vec2f st = (1 - uv[0] - uv[1]) * st0 + uv[0] * st1 + uv[1] * st2;
    return st;
}

std::optional<std::vector<Triangle>> Triangle::loadModel(const std::string &filepath)
{
    objl::Loader loader;
    if (!loader.LoadFile(filepath))
    {
        std::cout << "Failed to load model: " << filepath << std::endl;
        return std::nullopt;
    }
    std::vector<Triangle> triangles;
    for (const auto &mesh : loader.LoadedMeshes)
    {
        for (size_t i = 0; i < mesh.Vertices.size(); i += 3)
        {
            std::shared_ptr<Triangle> triangle = std::make_shared<Triangle>();
            for (int j = 0; j < 3; j++)
            {
                triangle->setVertex(j, cv::Vec3f(mesh.Vertices[i + j].Position.X, mesh.Vertices[i + j].Position.Y, mesh.Vertices[i + j].Position.Z));
                triangle->setNormal(j, cv::Vec3f(mesh.Vertices[i + j].Normal.X, mesh.Vertices[i + j].Normal.Y, mesh.Vertices[i + j].Normal.Z));
                triangle->setTexCoord(j, cv::Vec2f(mesh.Vertices[i + j].TextureCoordinate.X, mesh.Vertices[i + j].TextureCoordinate.Y));
            }
            triangles.push_back(*triangle);
        }
    }
    std::cout << "Loaded " << triangles.size() << " triangles" << std::endl;
    return triangles;
}
