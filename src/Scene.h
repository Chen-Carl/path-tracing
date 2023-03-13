#ifndef __SCENE_H__
#define __SCENE_H__

#include <opencv2/opencv.hpp>
#include "common/BVH.h"
#include "common/Light.h"
#include "common/Camera.h"
#include "objects/Object.h"

class Scene
{
private:
    int m_maxDepth = 5;
    double m_epsilon = 0.00001;
    float m_russianRoulette = 0.8;

    Camera m_camera;
    cv::Vec3f m_bgColor;

    std::vector<std::shared_ptr<Object>> m_objects;
    std::vector<std::shared_ptr<Light>> m_lights;

public:
    Scene() { }
    Scene(const Camera &camera, const cv::Vec3f &bgColor);

    virtual void add(std::shared_ptr<Object> object);
    virtual void add(std::shared_ptr<Light> light);
    
    /**
     * @brief Cast a ray into the scene and return the color of the first object hit.
     * @param eyePos The position of the camera.
     * @param dir The direction of the ray (pixel - camera).
     * @param scene The scene to cast the ray into.
     * @param depth The number of bounces of the ray.
     * @return The color of the first object hit.
    */
    virtual cv::Vec3f castRay(const cv::Vec3f &eyePos, const cv::Vec3f &dir,  int depth) const;

    /**
     * @brief Trace a ray into the scene and return the closest object hit.
     * @param orig The origin of the ray.
     * @param dir The direction of the ray.
     * @param objects The objects to trace the ray into.
     * @return The closest object hit.
    */
    virtual std::optional<HitPayload> trace(const Ray &ray) const;

    /**
     * @brief Path tracing algorithm.
     * @param eyePos The position of the camera.
     * @param dir The direction of the ray (pixel - camera).
     * @param depth The number of bounces of the ray.
     * @return The color of the first object hit.
     */
    virtual cv::Vec3f pathTracing(const cv::Vec3f &eyePos, const cv::Vec3f &dir, int depth) const;

    const cv::Vec3f &getBgColor() const { return m_bgColor; }
    double getEpsilon() const { return m_epsilon; }
    const std::vector<std::shared_ptr<Object>> &getObjects() const { return m_objects; }
    const std::vector<std::shared_ptr<Light>> &getLights() const { return m_lights; }
    int getWidth() const { return m_camera.width; }
    int getHeight() const { return m_camera.height; }
    float getFov() const { return m_camera.fov; }
    int getMaxDepth() const { return m_maxDepth; }
    const cv::Vec3f &getEyePos() const { return m_camera.eyePos; }
    float getRussianRoulette() const { return m_russianRoulette; }

protected:
    std::pair<HitPayload, float> sampleLight() const;
};

class BVHScene : public Scene
{
private:
    std::shared_ptr<BVH> m_bvh;

    virtual std::optional<HitPayload> trace(const Ray &ray) const override;

public:
    BVHScene(const Camera &camera, const cv::Vec3f &bgColor);

    void buildBVH();

    virtual cv::Vec3f castRay(const cv::Vec3f &eyePos, const cv::Vec3f &dir, int depth) const;

    virtual cv::Vec3f pathTracing(const cv::Vec3f &eyePos, const cv::Vec3f &dir, int depth) const override;

    friend void testSphereBVH();
};

#endif