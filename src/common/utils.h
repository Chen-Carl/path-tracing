#ifndef __COMMON_UTILS_H__
#define __COMMON_UTILS_H__

#include <cmath>
#include <optional>
#include <opencv2/opencv.hpp>

namespace zoe {

float deg2rad(float deg);

/**
 * @brief Solve quadratic equation
 * @return if the equation has no solution, return std::nullopt
 *         if the equation has solutions, return std::make_pair(x, y)
 */
std::optional<std::pair<float, float>> solveQuad(float a, float b, float c);

cv::Vec3f reflect(const cv::Vec3f &viewDir, const cv::Vec3f &normal);

cv::Vec3f refract(const cv::Vec3f &viewDir, const cv::Vec3f &normal, float ior);

/**
 * @brief Calculate the fresnel reflectance F(wo, wh, ior)
 * @param viewDir the incident view direction
 * @param normal the normal at the intersection point
 * @param ior the material refractive index
 */
float fresnel(const cv::Vec3f &viewDir, const cv::Vec3f &normal, float ior);

}

#endif