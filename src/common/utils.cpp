#include "common/utils.h"

namespace zoe {

float deg2rad(float deg)
{
    return deg * M_PI / 180.0f;
}

/**
 * @brief Solve quadratic equation
 * @return if the equation has no solution, return std::nullopt
 *         if the equation has solutions, return std::make_pair(x, y)
 */
std::optional<std::pair<float, float>> solveQuad(float a, float b, float c)
{
    float discr = b * b - 4 * a * c;
    if (discr == 0)
    {
        return std::make_pair(-0.5f * b / a, -0.5f * b / a);
    }
    else if (discr > 0)
    {
        float q = (b > 0) ? -0.5f * (b + std::sqrt(discr)) : -0.5f * (b - std::sqrt(discr));
        if (q / a > c / q)
        {
            return std::make_pair(c / q, q / a);
        }
        return std::make_pair(q / a, c / q);
    }
    return std::nullopt;
}

cv::Vec3f reflect(const cv::Vec3f &viewDir, const cv::Vec3f &normal)
{
    return viewDir - 2 * viewDir.dot(normal) * normal;
}

cv::Vec3f refract(const cv::Vec3f &viewDir, const cv::Vec3f &normal, float ior)
{
    float cosi = std::max(-1.0f, std::min(1.0f, viewDir.dot(normal)));
    float etai = 1, etat = ior;
    cv::Vec3f n = normal;
    if (cosi < 0)
    {
        cosi = -cosi;
    }
    else
    {
        std::swap(etai, etat);
        n = -normal;
    }
    float eta = etai / etat;
    float k = 1 - eta * eta * (1 - cosi * cosi);
    return k < 0 ? cv::Vec3f(0, 0, 0) : eta * viewDir + (eta * cosi - std::sqrt(k)) * n;
}

/**
 * @brief Calculate the fresnel reflectance F(wo, wh, ior)
 * @param viewDir the incident view direction
 * @param normal the normal at the intersection point
 * @param ior the material refractive index
 */
float fresnel(const cv::Vec3f &viewDir, const cv::Vec3f &normal, float ior)
{
    float cosi = std::max(-1.0f, std::min(1.0f, viewDir.dot(normal)));
    float etai = 1, etat = ior;
    if (cosi > 0)
    {
        std::swap(etai, etat);
    }
    float sint = etai / etat * std::sqrt(std::max(0.0f, 1 - cosi * cosi));
    if (sint >= 1)
    {
        return 1;
    }
    else
    {
        float cost = std::sqrt(std::max(0.0f, 1 - sint * sint));
        cosi = std::abs(cosi);
        float Rs = ((etat * cosi) - (etai * cost)) / ((etat * cosi) + (etai * cost));
        float Rp = ((etai * cosi) - (etat * cost)) / ((etai * cosi) + (etat * cost));
        return (Rs * Rs + Rp * Rp) / 2;
    }
    return -1;
}

}