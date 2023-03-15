#include "Object.h"
#include "common/utils.h"

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
    switch (m_material.materialType)
    {
        case Material::MaterialType::DIFFUSE_AND_GLOSSY:
        {
            float cosTheta = std::clamp(normal.dot(wo), 0.0f, 1.0f);
            if (cosTheta > 0)
            {
                return m_material.kd / M_PI;
            }
            return cv::Vec3f(0, 0, 0);
        }
        case Material::MaterialType::REFLECTION:
        {
            float cosTheta = normal.dot(wo);
            if (normal.dot(wo) > 0)
            {
                float kr = zoe::fresnel(wi, normal, getIor());
                if (cosTheta < 0.001)
                {
                    return cv::Vec3f(0, 0, 0);
                }
                return cv::Vec3f(kr / cosTheta, kr / cosTheta, kr / cosTheta);
            }
            return cv::Vec3f(0, 0, 0);
        }
        case Material::MaterialType::REFLECTION_AND_REFRACTION:
        {
            float cosTheta = normal.dot(wo);
            if (cosTheta > 0)
            {
                float kr = zoe::fresnel(wi, normal, getIor());
                return cv::Vec3f(kr / cosTheta, kr / cosTheta, kr / cosTheta);
            }
            else if (cosTheta < 0)
            {
                float kt = 1 - zoe::fresnel(wi, normal, getIor());
                cv::Vec3f res(kt / -cosTheta, kt / -cosTheta, kt / -cosTheta);
                if (m_material.tr != cv::Vec3f(0, 0, 0))
                {
                    return res.mul(m_material.tr);
                }
                return res;
            }
            return cv::Vec3f(0, 0, 0);
        }
    }
    throw std::runtime_error("Unsupported material type.");
}
