#include <numeric>
#include "common/OBJ_Loader.h"
#include "objects/MeshTriangles.h"
#include "MeshTriangles.h"

MeshTriangles::MeshTriangles()
{
    m_triangles = std::vector<Triangle>();
}

MeshTriangles::MeshTriangles(const std::vector<Triangle> &triangles)
{
    m_triangles = triangles;
}

std::optional<HitPayload> MeshTriangles::intersect(const cv::Vec3f &orig, const cv::Vec3f &dir) const
{
    std::optional<HitPayload> res = std::nullopt;
    for (const auto &triangle : m_triangles)
    {
        std::optional<HitPayload> hit = triangle.intersect(orig, dir);
        if (hit.has_value())
        {
            if (!res.has_value() || hit->dist < res->dist)
                res = hit;
        }
    }
    return res;
}

AABB MeshTriangles::getAABB() const
{
    AABB aabb = std::accumulate(m_triangles.begin(), m_triangles.end(), AABB{}, [](const AABB &acc, const auto &triangle) {
        return acc + triangle.getAABB();
    });
    return aabb;
}

std::optional<std::vector<Triangle>> MeshTriangles::loadModel(const std::string &filepath)
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
