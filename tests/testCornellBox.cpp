#include "objects/Triangle.h"
#include "common/Camera.h"
#include "Scene.h"
#include "Renderer.h"

int main(int argc, char** argv)
{
    // Change the definition here to change resolution
    BVHScene scene(600, 600);

    auto floor = Triangle::loadModel("models/cornellbox/floor.obj");
    auto shortbox = Triangle::loadModel("models/cornellbox/shortbox.obj");
    auto tallbox = Triangle::loadModel("models/cornellbox/tallbox.obj");
    auto left = Triangle::loadModel("models/cornellbox/left.obj");
    auto right = Triangle::loadModel("models/cornellbox/right.obj");
    auto lights = Triangle::loadModel("models/cornellbox/light.obj");

#define XX(mesh, material) \
    for (auto &tri : mesh.value()) \
    { \
        tri.setMaterial(material); \
        scene.add(std::make_shared<Triangle>(tri)); \
    }

    XX(floor, white);
    XX(shortbox, white);
    XX(tallbox, white);
    XX(left, red);
    XX(right, green);
    XX(lights, light);
#undef XX

    Camera camera {
        cv::Vec3f(0.843137, 0.67451, 0.235294),
        cv::Vec3f(278, 273, -800),
        40.0
    };

    scene.setCamera(camera);
    scene.buildBVH();

    RayTracer renderer(64);
    cv::Mat3f res = renderer.render(scene);
    cv::imwrite("testCornellBox.png", res * 255);

    return 0;
}