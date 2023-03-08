#include "Object.h"

Object::Object()
{
    m_diffuseColor = cv::Vec3f(0, 0, 0);
}

Object::Object(cv::Vec3f diffuseColor, Material material) :
    m_diffuseColor(diffuseColor),
    m_material(material)
{

}


Object::Object(cv::Vec3f diffuseColor, Material::MaterialType materialType, float kd, float ks, float specularExp, float ior) :
    m_diffuseColor(diffuseColor)
{
    m_material.emission = cv::Vec3f(0, 0, 0);
    m_material.ior = ior;
    m_material.kd = kd;
    m_material.ks = ks;
    m_material.specularExp = specularExp;
    m_material.materialType = materialType;
}

cv::Vec3f Object::evalLightContri(const cv::Vec3f &normal, const cv::Vec3f &wi, const cv::Vec3f &wo) const
{
    assert(m_material.materialType == Material::MaterialType::DIFFUSE_AND_GLOSSY);
    double cosTheta = normal.dot(wo);
    if (normal.dot(wo) > 0)
    {
        return m_material.kd / M_PI;
    }
    return cv::Vec3f(0, 0, 0);
}
