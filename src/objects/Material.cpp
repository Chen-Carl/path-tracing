#include "common/utils.h"
#include "objects/Material.h"

Material::Material(MaterialType materialType, cv::Vec3f emission, cv::Vec3f kd, cv::Vec3f ks, cv::Vec3f tr, float specularExp, float ior) :
    materialType(materialType),
    emission(emission),
    kd(kd),
    ks(ks),
    tr(tr),
    specularExp(specularExp),
    ior(ior)
{

}

float Material::pdf(const cv::Vec3f &normal, const cv::Vec3f &wi, const cv::Vec3f &wo)
{
    switch (materialType)
    {
        case MaterialType::DIFFUSE_AND_GLOSSY:
        {
            return wo.dot(normal) > 0.0f ? 0.5f / M_PI : 0.0f;
        }
        case MaterialType::REFLECTION:
        {
            return wo.dot(normal) > 0.0f ? 1.0f : 0.0f;
        }
        case MaterialType::REFLECTION_AND_REFRACTION:
        {
            float fr = zoe::fresnel(wi, normal, ior);
            return wo.dot(normal) > 0.0f ? fr : 1.0f - fr;
        }
        case MaterialType::DIFFUSE_AND_REFLECTION:
        {
            float cosTheta = wo.dot(normal);
            return cosTheta > 0.0f ? 0.5f / M_PI : 0.0f;
        }
        case MaterialType::DIFFUSE_AND_REFRACTION:
        {
            float fr = zoe::fresnel(wi, normal, ior);
            return wo.dot(normal) > 0.0f ? fr : 1.0f - fr;
        }
    }
    throw std::runtime_error("Unsupported material type.");
}

cv::Vec3f Material::sampleDir(const cv::Vec3f &normal, const cv::Vec3f &wi) const
{
    switch (materialType)
    {
        case Material::MaterialType::DIFFUSE_AND_GLOSSY:
        {
            float x = zoe::randomFloat();
            float y = zoe::randomFloat();
            float z = std::fabs(1 - 2 * x);
            float r = std::sqrt(1 - z * z);
            float phi = 2 * M_PI * y;
            cv::Vec3f localRay(r * std::cos(phi), r * std::sin(phi), z);
            return zoe::localToWorld(localRay, normal);
        }
        case Material::MaterialType::REFLECTION:
        {
            return zoe::reflect(wi, normal);
        }
        case Material::MaterialType::REFLECTION_AND_REFRACTION:
        {
            float fr = zoe::fresnel(wi, normal, ior);
            if (zoe::randomFloat() < fr)
            {
                return zoe::reflect(wi, normal);
            }
            else
            {
                return zoe::refract(wi, normal, ior);
            }
        }
        case Material::MaterialType::DIFFUSE_AND_REFLECTION:
        {
            float x = zoe::randomFloat();
            float y = zoe::randomFloat();
            float z = std::fabs(1 - 2 * x);
            float r = std::sqrt(1 - z * z);
            float phi = 2 * M_PI * y;
            cv::Vec3f localRay(r * std::cos(phi), r * std::sin(phi), z);
            return zoe::localToWorld(localRay, normal);
        }
        case Material::MaterialType::DIFFUSE_AND_REFRACTION:
        {
            if (zoe::randomFloat() > 0.9)
            {
                float x = zoe::randomFloat();
                float y = zoe::randomFloat();
                float z = std::fabs(1 - 2 * x);
                float r = std::sqrt(1 - z * z);
                float phi = 2 * M_PI * y;
                cv::Vec3f localRay(r * std::cos(phi), r * std::sin(phi), z);
                return zoe::localToWorld(localRay, normal);
            }
            return zoe::refract(wi, normal, ior);
        }
    }
    throw std::runtime_error("Unsupported material type.");
}

cv::Vec3f Material::specularBRDF(const cv::Vec3f &normal, const cv::Vec3f &wi, const cv::Vec3f &wo) const
{
    float cosTheta = wo.dot(normal);
    float cosAlpha = cv::normalize(wo - wi).dot(normal);
    return ks * specularExp * std::pow(cosAlpha, specularExp) / (2 * M_PI);
}

cv::Vec3f Material::lambertianBRDF(const cv::Vec3f &normal, const cv::Vec3f &wi, const cv::Vec3f &wo) const
{
    if (wo.dot(normal) > 0)
    {
        return 0.5 * kd / M_PI;
    }
    return cv::Vec3f(0.0f, 0.0f, 0.0f);
}