#include "Scene.h"

Scene::Scene(int width, int height) : 
    m_width(width), 
    m_height(height)
{
    m_bgColor = cv::Vec3f(0.843137, 0.67451, 0.235294);
    m_objects = std::vector<std::shared_ptr<Object>>();
    m_lights = std::vector<std::shared_ptr<Light>>();
}

void Scene::add(std::shared_ptr<Object> object)
{
    m_objects.push_back(object);
}

void Scene::add(std::shared_ptr<Light> light)
{
    m_lights.push_back(light);
}

BVHScene::BVHScene(int width, int height) : Scene(width, height)
{
    
}

void BVHScene::buildBVH()
{
    std::cout << "Building BVH..." << std::endl;
    m_bvh = std::make_shared<BVH>(getObjects());
}
