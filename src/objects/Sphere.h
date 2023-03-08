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

    virtual std::optional<HitPayload> intersect(const Ray &ray) const override;

    virtual AABB getAABB() const override;

    virtual cv::Vec3f getNormal(const cv::Vec3f &point) const override;
    virtual float getArea() const override;
    virtual std::pair<HitPayload, float> samplePoint() const override;
    virtual cv::Vec3f sampleDir(const cv::Vec3f &normal, const cv::Vec3f &wi) const override;
};

#endif