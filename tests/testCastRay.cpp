#include "common/Camera.h"
#include "Scene.h"
#include "Renderer.h"
#include "objects/Sphere.h"


int main()
{
    Camera camera(1280, 960, 90.0f);
    Scene scene(camera, cv::Vec3f(0.843137, 0.67451, 0.235294));
    std::shared_ptr<Object> sph1 = std::make_shared<Sphere>(cv::Vec3f(-1, 0, -12), 2);
    sph1->setMaterialType(Material::MaterialType::DIFFUSE_AND_GLOSSY);
    sph1->setDiffuseColor(cv::Vec3f(0.8, 0.7, 0.6));

    std::shared_ptr<Object> sph2 = std::make_shared<Sphere>(cv::Vec3f(0.5, -0.5, -8), 1.5);
    sph2->setIor(1.5);
    sph2->setMaterialType(Material::MaterialType::REFLECTION);

    scene.add(std::move(sph1));
    // scene.add(std::move(sph2));

    scene.add(std::make_shared<Light>(cv::Vec3f(-20, 70, 20), cv::Vec3f(0.5, 0.5, 0.5)));
    scene.add(std::make_shared<Light>(cv::Vec3f(30, 50, -12), cv::Vec3f(0.5, 0.5, 0.5)));

    auto res = scene.castRay(cv::Vec3f(0, 0, 0), cv::Vec3f(-0.103125, 0.167708, -1), 0);
    std::cout << res << std::endl;

    return 0;
}
