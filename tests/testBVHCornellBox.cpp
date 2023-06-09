#include "objects/Triangle.h"
#include "common/Camera.h"
#include "Scene.h"
#include "Renderer.h"

int main(int argc, char** argv)
{
    Camera camera(120, 120, 40.0f, cv::Vec3f(275, 274, -800), cv::Vec3f(275, 274, -799), cv::Vec3f(0, 1, 0));
    camera.init();
    
    BVHScene scene(camera, cv::Vec3f(0.843137, 0.67451, 0.235294));

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

    XX(floor, zoe::white);
    XX(shortbox, zoe::glass);
    XX(tallbox, zoe::white);
    XX(left, zoe::red);
    XX(right, zoe::green);
    XX(lights, zoe::light);
#undef XX

    scene.buildBVH();

    RayTracer renderer(128, 12);
    cv::Mat3f res = renderer.render(scene);
    cv::imwrite("output/testBVHCornellBox.png", res * 255);

    return 0;
}