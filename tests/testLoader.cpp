#include <iostream>
#include "objects/Triangle.h"
#include "objects/ModelLoader.h"
#include "Scene.h"
#include "Renderer.h"

int main()
{
    BVHScene scene = ModelLoader::loadBVHScene("models/cornellbox-tc/cornell-box.obj");
    scene.buildBVH();

    RayTracer renderer(8, 2);
    cv::Mat3f image = renderer.render(scene);
    cv::imwrite("testLoader.png", image * 255);

    return 0;
}




    

    

    