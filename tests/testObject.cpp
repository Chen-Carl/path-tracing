#include "objects/Sphere.h"

void testSphere();

int main()
{
    testSphere();
    return 0;
}

void testSphere()
{
    std::cout << "========== testSphere ==========" << std::endl;
    std::shared_ptr<Object> sph = std::make_shared<Sphere>(cv::Vec3f(0, 0, 0), 1.0);
    auto res = sph->intersect(cv::Vec3f(0, 0, 0), cv::Vec3f(1, 1, 0));
    if (res.has_value())
    {
        auto [t, id, uv] = res.value();
        std::cout << "t = " << t << ", id = " << id << ", uv = " << uv << std::endl;
    }

    res = sph->intersect(cv::Vec3f(0, 0, 0), cv::Vec3f(1, 1, 1));
    if (res.has_value())
    {
        auto [t, id, uv] = res.value();
        std::cout << "t = " << t << ", id = " << id << ", uv = " << uv << std::endl;
    }
}