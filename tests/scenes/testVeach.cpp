#include <iostream>
#include "objects/Triangle.h"
#include "objects/ModelLoader.h"
#include "Scene.h"
#include "Renderer.h"

int main()
{
    BVHScene scene = ModelLoader::loadBVHScene("models/veachmis/veach-mis.obj");
    scene.buildBVH();

    RayTracer renderer(32, 1);
    cv::Mat3f image = renderer.render(scene);
    cv::imwrite("output/veachmis/testVeach-32.png", image * 255);

    return 0;
}
