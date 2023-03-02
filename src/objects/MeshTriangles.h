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

    virtual std::optional<HitPayload> intersect(const cv::Vec3f &orig, const cv::Vec3f &dir) const override;

    virtual AABB getAABB() const override;

    static std::optional<std::vector<Triangle>> loadModel(const std::string &filepath);
};

#endif