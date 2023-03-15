#include <iostream>
#include "objects/Triangle.h"
#include "objects/ModelLoader.h"
#include "Scene.h"
#include "Renderer.h"

int main()
{
    BVHScene scene = ModelLoader::loadBVHScene("models/stairscase/stairscase.obj");
    scene.buildBVH();

    RayTracer renderer(1, 10);
    cv::Mat3f image = renderer.render(scene);
    cv::imwrite("testStairscase.png", image * 255);

    return 0;
}
