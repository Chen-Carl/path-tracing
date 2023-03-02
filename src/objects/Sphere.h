#ifndef __OBJECTS_SPHERE_H__
#define __OBJECTS_SPHERE_H__

#include "objects/Object.h"

class Sphere : public Object
{
private:
    cv::Vec3f m_center;
    float m_radius;

public:
    Sphere(const cv::Vec3f &center, float radius);

    virtual std::optional<HitPayload> intersect(const cv::Vec3f &orig, const cv::Vec3f &dir) const override;

    virtual AABB getAABB() const override;

    virtual cv::Vec3f getNormal(const cv::Vec3f &point) const override;
};

#endif