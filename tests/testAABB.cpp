#include "common/AABB.h"

int main()
{
    AABB aabb(cv::Vec3f(-5, -3, -16), cv::Vec3f(5, -3, -6));
    bool res = aabb.intersect(Ray(cv::Vec3f(0, 0, 0), cv::Vec3f(0.109375, -0.338542, -1)));

    if (res)
    {
        std::cout << "Hit!" << std::endl;
    }
    else
    {
        std::cout << "Miss!" << std::endl;
    }

    return 0;
}