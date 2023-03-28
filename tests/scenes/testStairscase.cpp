#include <iostream>
#include "objects/Triangle.h"
#include "objects/ModelLoader.h"
#include "Scene.h"
#include "Renderer.h"

int main()
{
    std::string sceneName = "models/stairscase/stairscase.obj";
    std::string ckpt = "output/stairscase/testStairscase-256.png";
    BVHScene scene = ModelLoader::loadBVHScene(sceneName);
    scene.buildBVH();

    RayTracer renderer(768, 1);
    cv::Mat3f image = renderer.render(scene, ckpt);
    cv::imwrite("output/stairscase/testStairscase-1024.png", image * 255);

    return 0;
}
