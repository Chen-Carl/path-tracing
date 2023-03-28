#include <iostream>
#include "objects/Triangle.h"
#include "objects/ModelLoader.h"
#include "Scene.h"
#include "Renderer.h"

int main()
{
    std::string sceneName = "models/cornellbox-tc/cornell-box.obj";
    std::string ckpt = "output/cornellbox/testCornell-1024.png";

    BVHScene scene = ModelLoader::loadBVHScene(sceneName);
    scene.buildBVH();

    RayTracer renderer(3072, 16);
    cv::Mat3f image = renderer.render(scene, ckpt);
    cv::imwrite("output/cornellbox/testCornell-4096.png", image * 255);

    return 0;
}
