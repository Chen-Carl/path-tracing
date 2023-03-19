#ifndef __OBJECTS_MATERIAL_H__
#define __OBJECTS_MATERIAL_H__

#include <opencv2/opencv.hpp>

class Material
{
public:
    enum class MaterialType
    {
        DIFFUSE_AND_GLOSSY,
        REFLECTION_AND_REFRACTION,
        REFLECTION,
        DIFFUSE_AND_REFLECTION,
        DIFFUSE_AND_REFRACTION
    };

public:
    MaterialType materialType;
    cv::Vec3f emission;             // emission intensity
    cv::Vec3f kd;                   // Phong shading diffuse coefficient
    cv::Vec3f ks;                   // Phong shading specular coefficient
    cv::Vec3f tr;                   // Transmission coefficient
    float ior;                      // Index of refraction
    float specularExp;              // Controll the size of specular highlight

    Material(MaterialType materialType = MaterialType::DIFFUSE_AND_GLOSSY, 
        cv::Vec3f emission = cv::Vec3f(0, 0, 0), 
        cv::Vec3f kd = cv::Vec3f(0.8, 0.8, 0.8), 
        cv::Vec3f ks = cv::Vec3f(0.2, 0.2, 0.2), 
        cv::Vec3f tr = cv::Vec3f(1, 1, 1),
        float specularExp = 25.0, 
        float ior = 1.3
    );

    float pdf(const cv::Vec3f &normal, const cv::Vec3f &wi, const cv::Vec3f &wo);

    cv::Vec3f sampleDir(const cv::Vec3f &normal, const cv::Vec3f &wi) const;

    cv::Vec3f specularBRDF(const cv::Vec3f &normal, const cv::Vec3f &wi, const cv::Vec3f &wo) const;

    cv::Vec3f lambertianBRDF(const cv::Vec3f &normal, const cv::Vec3f &wi, const cv::Vec3f &wo) const;
};

namespace zoe {

static Material red(Material::MaterialType::DIFFUSE_AND_GLOSSY, cv::Vec3f(0.0f), cv::Vec3f(0.05f, 0.065f, 0.63f));
static Material green(Material::MaterialType::DIFFUSE_AND_GLOSSY, cv::Vec3f(0.0f), cv::Vec3f(0.091f, 0.45f, 0.14f));
static Material white(Material::MaterialType::DIFFUSE_AND_GLOSSY, cv::Vec3f(0.0f), cv::Vec3f(0.68f, 0.71f, 0.725f));
static Material light(Material::MaterialType::DIFFUSE_AND_GLOSSY, (
        8.0f * cv::Vec3f(0.747f, 0.747f + 0.258f, 0.747f + 0.058f) 
        + 15.6f * cv::Vec3f(0.740f, 0.740f + 0.160f, 0.740f + 0.287f) 
        + 18.4f * cv::Vec3f(0.737f, 0.737f + 0.159f, 0.737f + 0.642f)
    ), cv::Vec3f(0.65f, 0.65f, 0.65f));
static Material mirror(Material::MaterialType::REFLECTION, cv::Vec3f(0.0f, 0.0f, 0.0f), cv::Vec3f(0.3f, 0.3f, 0.25f), cv::Vec3f(0.45f, 0.45f, 0.45f), cv::Vec3f(0.0f, 0.0f, 0.0f), 20.0f, 12.85f);
static Material glass(Material::MaterialType::REFLECTION_AND_REFRACTION, cv::Vec3f(0.0f, 0.0f, 0.0f), cv::Vec3f(0.3f, 0.3f, 0.25f), cv::Vec3f(0.45f, 0.45f, 0.45f), cv::Vec3f(0.0f, 0.0f, 0.0f), 10.0f, 12.85f);

}

#endif