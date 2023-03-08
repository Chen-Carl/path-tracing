#include "Material.h"

Material::Material(MaterialType materialType, cv::Vec3f emission, cv::Vec3f kd, cv::Vec3f ks, float specularExp, float ior) :
    materialType(materialType),
    emission(emission),
    kd(kd),
    ks(ks),
    specularExp(specularExp),
    ior(ior)
{

}
