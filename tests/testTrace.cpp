#include "Scene.h"
#include "Renderer.h"
#include "objects/Sphere.h"


int main()
{
    Scene scene(1280, 960);
    std::shared_ptr<Object> sph1 = std::make_shared<Sphere>(cv::Vec3f(-1, 0, -12), 2);
    sph1->setMaterialType(Material::MaterialType::DIFFUSE_AND_GLOSSY);
    sph1->setDiffuseColor(cv::Vec3f(0.8, 0.7, 0.6));

    std::shared_ptr<Object> sph2 = std::make_shared<Sphere>(cv::Vec3f(0.5, -0.5, -8), 1.5);
    sph2->setIor(1.5);
    sph2->setMaterialType(Material::MaterialType::REFLECTION_AND_REFRACTION);

    scene.add(std::move(sph1));
    scene.add(std::move(sph2));

    Camera camera {
        cv::Vec3f(0.843137, 0.67451, 0.235294),
        cv::Vec3f(0, 0, 0),
        90.0f
    };

    scene.setCamera(camera);

    scene.add(std::make_shared<Light>(cv::Vec3f(-20, 70, 20), cv::Vec3f(0.5, 0.5, 0.5)));
    scene.add(std::make_shared<Light>(cv::Vec3f(30, 50, -12), cv::Vec3f(0.5, 0.5, 0.5)));

    auto res = scene.trace(Ray(cv::Vec3f(-1.1874, 1.93103, -11.5142), cv::Vec3f(-0.116493, 0.30546, -0.965342)), scene.getObjects());

    if (res.has_value())
    {
        std::cout << res->dist << std::endl;
    }

    return 0;
}
