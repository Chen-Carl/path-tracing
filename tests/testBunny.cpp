#include "objects/MeshTriangles.h"
#include "Scene.h"
#include "Light.h"
#include "Renderer.h"

int main()
{
    BVHScene scene(1280, 960);

    std::optional<std::vector<Triangle>> triangles = MeshTriangles::loadModel("models/bunny.obj");
    if (!triangles.has_value())
    {
        std::cout << "Failed to load model" << std::endl;
        return 0;
    }
    for (auto &triangle : triangles.value())
    {
        scene.add(std::make_shared<Triangle>(triangle));
    }
    scene.add(std::make_shared<Light>(cv::Vec3f(-20, 70, 20), 1));
    scene.add(std::make_shared<Light>(cv::Vec3f(20, 70, 20), 1));
    scene.buildBVH();

    Renderer renderer;
    cv::Mat3f res = renderer.render(scene);
    cv::imwrite("testBunny.png", res);
    return 0;
}