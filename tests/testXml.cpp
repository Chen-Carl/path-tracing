#include "objects/ModelLoader.h"

int main()
{
    auto [camera, lights] = ModelLoader::loadXML("models/stairscase/stairscase.xml");
    std::cout << "Camera: " << std::endl;
    std::cout << "  width: " << camera.width << std::endl;
    std::cout << "  height: " << camera.height << std::endl;
    std::cout << "  fov: " << camera.fov << std::endl;
    std::cout << "  eyePos: " << camera.eyePos << std::endl;
    std::cout << "  lookat: " << camera.lookat << std::endl;
    std::cout << "  up: " << camera.up << std::endl;
    std::cout << "Lights: " << std::endl;
    for (auto &light : lights)
    {
        std::cout << "  " << light.first << ": " << light.second << std::endl;
    }
    return 0;
}