#ifndef __OBJECTS_OBJECT_H__
#define __OBJECTS_OBJECT_H__

#include <opencv2/opencv.hpp>
#include "common/AABB.h"
#include "common/Ray.h"
#include "objects/Material.h"
#include "objects/HitPayload.h"

class Object : public std::enable_shared_from_this<const Object>
{
private:
    cv::Vec3f m_diffuseColor;   // color of diffuse light
    Material m_material;        // material of the object
    std::string m_texturePath;  // texture name of the object
    std::shared_ptr<const cv::Mat3f> m_texture = nullptr;   // texture of the object

public:
    Object();
    Object(cv::Vec3f diffuseColor, Material material);
    Object(cv::Vec3f diffuseColor, Material::MaterialType materialType, float kd = 0.8, float ks = 0.2, float specularExp = 25.0, float ior = 1.3);
    virtual ~Object() = default;

    virtual cv::Vec3f evalLightBRDF(const cv::Vec3f &normal, const cv::Vec3f &wi, const cv::Vec3f &wo) const;

    /**
     * @brief Intersect a ray with the object
     * @param orig The origin of the ray
     * @param dir The direction of the ray
     */
    virtual std::optional<HitPayload> intersect(const Ray &ray) const = 0;
    virtual AABB getAABB() const = 0;
    virtual cv::Vec3f getNormal(const cv::Vec3f &point) const = 0;
    virtual float getArea() const = 0;
    virtual HitPayload samplePoint() const = 0;
    virtual cv::Vec2f getTexCoords(const cv::Vec2f &uv) const = 0;

    virtual Material::MaterialType getMaterialType() const { return m_material.materialType; }
    virtual cv::Vec3f getDiffuseColor(const cv::Vec2f &st) const { return m_diffuseColor; }
    virtual float getIor() const { return m_material.ior; }
    virtual float getSpecularExp() const { return m_material.specularExp; }
    virtual cv::Vec3f getKd() const { return m_material.kd; }
    virtual cv::Vec3f getKs() const { return m_material.ks; }
    virtual cv::Vec3f getEmission() const { return m_material.emission; }
    virtual Material getMaterial() const { return m_material; }
    virtual std::string getTexturePath() const { return m_texturePath; }
    virtual std::shared_ptr<const cv::Mat3f> getTexture() const { return m_texture; }

    virtual void setMaterialType(Material::MaterialType materialType) { m_material.materialType = materialType; }
    virtual void setDiffuseColor(const cv::Vec3f &diffuseColor) { m_diffuseColor = diffuseColor; }
    virtual void setIor(float ior) { m_material.ior = ior; }
    virtual void setKd(cv::Vec3f kd) { m_material.kd = kd; }
    virtual void setKs(cv::Vec3f ks) { m_material.ks = ks; }
    virtual void setSpecularExp(float specularExp) { m_material.specularExp = specularExp; }
    virtual void setEmission(const cv::Vec3f &emission) { m_material.emission = emission; }
    virtual void setMaterial(const Material &material) { m_material = material; }
    virtual void setTexturePath(const std::string &texturePath) { m_texturePath = texturePath; }
    virtual void setTexture(std::shared_ptr<const cv::Mat3f> texture) { m_texture = texture; }

    virtual bool emissive() const { return m_material.emission != cv::Vec3f(0, 0, 0); }
};

#endif