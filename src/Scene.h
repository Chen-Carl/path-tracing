#ifndef __SCENE_H__
#define __SCENE_H__

#include <opencv2/opencv.hpp>
#include "common/BVH.h"
#include "objects/Object.h"
#include "Light.h"

class Scene
{
private:
    int m_width = 1280;
    int m_height = 960;
    float m_fov = 90.0f;
    int m_maxDepth = 5;
    double epsilon = 0.00001;
    cv::Vec3f m_bgColor;

    std::vector<std::shared_ptr<Object>> m_objects;
    std::vector<std::shared_ptr<Light>> m_lights;

public:
    Scene(int width = 1280, int height = 960);

    void add(std::shared_ptr<Object> object);
    void add(std::shared_ptr<Light> light);

    const std::vector<std::shared_ptr<Object>> &getObjects() const { return m_objects; }
    const std::vector<std::shared_ptr<Light>> &getLights() const { return m_lights; }
    int getWidth() const { return m_width; }
    int getHeight() const { return m_height; }
    float getFov() const { return m_fov; }
    int getMaxDepth() const { return m_maxDepth; }
    double getEpsilon() const { return epsilon; }
    const cv::Vec3f &getBgColor() const { return m_bgColor; }
};

class BVHScene : public Scene
{
private:
    std::shared_ptr<BVH> m_bvh;

public:
    BVHScene(int width = 1280, int height = 960);

    void buildBVH();
};

#endif