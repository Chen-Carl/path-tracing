#include "common/Camera.h"
#include "objects/MeshTriangles.h"
#include "Scene.h"
#include "Light.h"
#include "Renderer.h"

int main()
{
    BVHScene scene(1280, 960);

    std::optional<std::vector<Triangle>> triangles = Triangle::loadModel("models/bunny.obj");
    if (!triangles.has_value())
    {
        std::cout << "Failed to load model" << std::endl;
        return 0;
    }
    for (auto &triangle : triangles.value())
    {
        triangle.setMaterialType(Object::MaterialType::DIFFUSE_AND_GLOSSY);
        scene.add(std::make_shared<Triangle>(triangle));
    }
    scene.add(std::make_shared<Light>(cv::Vec3f(-20, 70, 20), 1));
    scene.add(std::make_shared<Light>(cv::Vec3f(20, 70, 20), 1));
    scene.buildBVH();

    Camera camera = {
        cv::Vec3f(0.843137, 0.67451, 0.235294),
        cv::Vec3f(0, 0.1, 0.4),
        45.0
    };

    scene.setCamera(camera);

    Renderer renderer;
    cv::Mat3f res = renderer.render(scene);
    cv::imwrite("testBunny.png", res * 255);
    return 0;
}