#include "Light.h"

Light::Light()
{
    m_pos = cv::Vec3f(0.0f, 0.0f, 0.0f);
    m_intensity = cv::Vec3f(0.0f, 0.0f, 0.0f);
}

Light::Light(cv::Vec3f pos, cv::Vec3f intensity) :
    m_pos(pos),
    m_intensity(intensity)
{

}