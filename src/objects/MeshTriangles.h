#ifndef __OBJECTS_MESHTRIANGLES_H__
#define __OBJECTS_MESHTRIANGLES_H__

#include "objects/Object.h"
#include "objects/Triangle.h"

class MeshTriangles : public Object
{
private:
    std::vector<Triangle> m_triangles;

public:
    MeshTriangles();
    MeshTriangles(const std::vector<Triangle> &triangles);

    virtual ~MeshTriangles() = default;

    virtual std::optional<HitPayload> intersect(const Ray &ray) const override;

    virtual AABB getAABB() const override;
};

#endif