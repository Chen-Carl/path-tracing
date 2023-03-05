#include <numeric>
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

std::optional<HitPayload> MeshTriangles::intersect(const Ray &ray) const
{
    std::optional<HitPayload> res = std::nullopt;
    for (const auto &triangle : m_triangles)
    {
        std::optional<HitPayload> hit = triangle.intersect(ray);
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
