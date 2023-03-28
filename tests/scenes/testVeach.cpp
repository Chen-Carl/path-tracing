#include <iostream>
#include "objects/Triangle.h"
#include "objects/ModelLoader.h"
#include "Scene.h"
#include "Renderer.h"

int main()
{
    std::string sceneName = "models/veachmis/veach-mis.obj";
    std::string ckpt = "output/veachmis/testVeach-1536.png";
    BVHScene scene = ModelLoader::loadBVHScene(sceneName);
    scene.buildBVH();

    RayTracer renderer(6656, 1);
    cv::Mat3f image = renderer.render(scene, ckpt);
    cv::imwrite("output/veachmis/testVeach-8192.png", image * 255);

    return 0;
}
