#ifndef __OBJECTS_OBJECT_H__
#define __OBJECTS_OBJECT_H__

#include <opencv2/opencv.hpp>
#include "common/AABB.h"
#include "common/Ray.h"
#include "objects/HitPayload.h"

class Object : public std::enable_shared_from_this<const Object>
{
public:
    enum class MaterialType
    {
        DIFFUSE_AND_GLOSSY,
        REFLECTION_AND_REFRACTION,
        REFLECTION
    };

private:
    cv::Vec3f m_diffuseColor;   // color of diffuse light
    MaterialType m_materialType;
    float m_kd;                 // Phong shading diffuse coefficient
    float m_ks;                 // Phong shading specular coefficient
    float m_specularExp;        // Controll the size of specular highlight
    float m_ior;                // Index of refraction

public:
    Object();
    Object(cv::Vec3f diffuseColor, MaterialType materialType, float kd = 0.8, float ks = 0.2, float specularExp = 25.0, float ior = 1.3);
    virtual ~Object() = default;

    /**
     * @brief Intersect a ray with the object
     * @param orig The origin of the ray
     * @param dir The direction of the ray
     */
    virtual std::optional<HitPayload> intersect(const Ray &ray) const = 0;

    virtual AABB getAABB() const = 0;

    virtual MaterialType getMaterialType() const { return m_materialType; }
    virtual cv::Vec3f getDiffuseColor(const cv::Vec2f &st) const { return m_diffuseColor; }
    virtual float getIor() const { return m_ior; }
    virtual float getSpecularExp() const { return m_specularExp; }
    virtual float getKd() const { return m_kd; }
    virtual float getKs() const { return m_ks; }
    virtual cv::Vec2f getStCoords(const cv::Vec2f &uv) const { return cv::Vec2f(0.0, 0.0); }
    virtual cv::Vec3f getNormal(const cv::Vec3f &point) const = 0;

    virtual void setMaterialType(MaterialType materialType) { m_materialType = materialType; }
    virtual void setDiffuseColor(const cv::Vec3f &diffuseColor) { m_diffuseColor = diffuseColor; }
    virtual void setIor(float ior) { m_ior = ior; }
    virtual void setKd(float kd) { m_kd = kd; }
    virtual void setKs(float ks) { m_ks = ks; }
    virtual void setSpecularExp(float specularExp) { m_specularExp = specularExp; }
};

#endif