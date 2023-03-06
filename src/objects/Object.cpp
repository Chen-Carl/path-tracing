#include "Object.h"

Object::Object()
{
    m_diffuseColor = cv::Vec3f(0, 0, 0);
    m_materialType = MaterialType::DIFFUSE_AND_GLOSSY;
    m_kd = 0.8;
    m_ks = 0.2;
    m_specularExp = 25.0;
    m_ior = 1.3;
}

Object::Object(cv::Vec3f diffuseColor, MaterialType materialType, float kd, float ks, float specularExp, float ior) :
    m_diffuseColor(diffuseColor),
    m_materialType(materialType),
    m_kd(kd),
    m_ks(ks),
    m_specularExp(specularExp),
    m_ior(ior)
{

}
