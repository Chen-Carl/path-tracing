#ifndef __COMMON_UTILS_H__
#define __COMMON_UTILS_H__

#include <cmath>
#include <optional>
#include <filesystem>
#include <opencv2/opencv.hpp>
#include <indicators/cursor_control.hpp>
#include <indicators/progress_bar.hpp>

#define OUTPUT_DEBUG_LOG false
#define ENABLE_OPENMP true

namespace zoe {

const float selfCrossEpsilon = 0.01;
const float lightFirstEpsilon = 0.01;
const float denominatorEpsilon = 0.00001;

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

float randomFloat();

cv::Vec3f localToWorld(const cv::Vec3f &dir, const cv::Vec3f &normal);

void updateProgress(float progress);

indicators::ProgressBar createProgressBar(const std::string &&desc, size_t barWidth);

std::string getLastFile(const std::string &directory);

}

#endif