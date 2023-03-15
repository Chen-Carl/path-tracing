#include "objects/Triangle.h"
#include "common/Camera.h"
#include "Scene.h"
#include "Renderer.h"

int main(int argc, char** argv)
{
    Camera camera(480, 480, 40.0f);
    camera.eyePos = cv::Vec3f(278, 273, -800);

    Scene scene(camera, cv::Vec3f(0.843137, 0.67451, 0.235294));

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

    RayTracer renderer(4, 8);
    cv::Mat3f res = renderer.render(scene);
    cv::imwrite("output/testCornellBox.png", res * 255);

    return 0;
}