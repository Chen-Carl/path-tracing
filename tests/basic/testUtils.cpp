#include <iostream>
#include <opencv2/opencv.hpp>
#include "common/utils.h"

void testSolveQuad();
void testReflect();
void testRefract();
void testFresnel();
void testLocalToWorld();
void testFilesystem();

int main()
{
    testSolveQuad();
    testReflect();
    testRefract();
    testFresnel();
    testLocalToWorld();
    testFilesystem();
    return 0;
}

void testSolveQuad()
{
    std::cout << "========== testSolveQuad ==========" << std::endl;
    auto res = zoe::solveQuad(3, 2, -1);
    if (res.has_value())
    {
        std::cout << "x = " << res.value().first << ", y = " << res.value().second << std::endl;
    }
    res = zoe::solveQuad(3, 2, 1);
    if (res.has_value())
    {
        std::cout << "x = " << res.value().first << ", y = " << res.value().second << std::endl;
    }
    else
    {
        std::cout << "No solution" << std::endl;
    }
}

void testReflect()
{
    std::cout << "========== testReflect ==========" << std::endl;
    cv::Vec3f res = zoe::reflect(cv::Vec3f(1, 1, 1), cv::Vec3f(0, 1, 0));
    std::cout << res << std::endl;
}

void testRefract()
{
    std::cout << "========== testRefract ==========" << std::endl;
    cv::Vec3f res = zoe::refract(cv::Vec3f(1, 1, 1), cv::Vec3f(0, 1, 0), 1.5);
    std::cout << res << std::endl;
}

void testFresnel()
{
    std::cout << "========== testFresnel ==========" << std::endl;
    float res = zoe::fresnel(cv::Vec3f(1, 1, 1), cv::Vec3f(0, 1, 0), 1.5);
    std::cout << res << std::endl;
}

void testLocalToWorld()
{
    std::cout << "========== testLocalToWorld ==========" << std::endl;
    cv::Vec3f res = zoe::localToWorld(cv::Vec3f(13, 7, 2), cv::Vec3f(2, 3, 4));
    std::cout << res << std::endl;
}

void testFilesystem()
{
    std::cout << "========== testFilesystem ==========" << std::endl;
    std::cout << zoe::getLastFile("output/cornellbox") << std::endl;
}