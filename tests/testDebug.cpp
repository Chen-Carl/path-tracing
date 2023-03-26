#include <iostream>
#include "objects/Triangle.h"
#include "objects/ModelLoader.h"
#include "Scene.h"
#include "Renderer.h"

int main()
{
    BVHScene scene = ModelLoader::loadBVHScene("models/stairscase/stairscase.obj");

    scene.buildBVH();

    RayTracer renderer(1, 1);
    cv::Vec3f dir = cv::Vec3f(-1.15034, -0.235226, 0.0815291);
    cv::Vec3f eyePos = cv::Vec3f(6.91182, 1.65163, 2.55414);
    cv::Vec3f color = scene.pathTracing(eyePos, dir);
    std::cout << color << std::endl;

    return 0;
}
