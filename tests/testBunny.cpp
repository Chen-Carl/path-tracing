#include "common/Camera.h"
#include "objects/Triangle.h"
#include "Scene.h"
#include "Light.h"
#include "Renderer.h"

int main()
{
    BVHScene scene(1280, 960);

    std::optional<std::vector<Triangle>> triangles = Triangle::loadModel("models/bunny/bunny.obj");
    if (!triangles.has_value())
    {
        std::cout << "Failed to load model" << std::endl;
        return 0;
    }
    for (auto &triangle : triangles.value())
    {
        triangle.setMaterialType(Material::MaterialType::DIFFUSE_AND_GLOSSY);
        triangle.setDiffuseColor(cv::Vec3f(0.5, 0.5, 0.5));
        triangle.setKd(cv::Vec3f(0.6, 0.6, 0.6));
        triangle.setKs(cv::Vec3f(0, 0, 0));
        triangle.setSpecularExp(0.0);
        scene.add(std::make_shared<Triangle>(triangle));
    }
    scene.add(std::make_shared<Light>(cv::Vec3f(-20, 70, 20), cv::Vec3f(1, 1, 1)));
    scene.add(std::make_shared<Light>(cv::Vec3f(20, 70, 20), cv::Vec3f(1, 1, 1)));

    Camera camera = {
        cv::Vec3f(0.843137, 0.67451, 0.235294),
        cv::Vec3f(0, 0.1, 0.4),
        45.0
    };

    scene.setCamera(camera);

    Renderer renderer;
    cv::Mat3f res = renderer.render(scene);
    cv::imwrite("testBunny.png", res * 255);

    // cv::Vec3f color = scene.castRay(cv::Vec3f(0, 0.1, 0.4), cv::Vec3f(0.0107603, -0.069296, -0.997538), 0);
    // std::cout << color << std::endl;

    return 0;
}