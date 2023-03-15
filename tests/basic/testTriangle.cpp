#include "objects/Triangle.h"

int main()
{
    std::shared_ptr<Triangle> tri = std::make_shared<Triangle>(std::array<cv::Vec3f, 3>({
        cv::Vec3f(-5, -3, -6),
        cv::Vec3f(5, -3, -6),
        cv::Vec3f(-5, -3, -16)
    }));

    Ray ray(cv::Vec3f(0, 0, 0), cv::Vec3f(0.109375, -0.338542, -1));

    auto res = tri->intersect(ray);
    if (res.has_value())
    {
        std::cout << "Hit!" << std::endl;
    }
    else
    {
        std::cout << "Miss!" << std::endl;
    }

    return 0;
}