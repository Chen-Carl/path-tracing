#ifndef __SCENE_H__
#define __SCENE_H__

#include <opencv2/opencv.hpp>
#include "common/BVH.h"
#include "common/Camera.h"
#include "objects/Object.h"
#include "Light.h"

class Scene
{
private:
    int m_width = 1280;
    int m_height = 960;
    float m_fov = 90.0f;
    int m_maxDepth = 5;
    double m_epsilon = 0.00001;
    float m_russianRoulette = 0.8;
    cv::Vec3f m_bgColor;
    cv::Vec3f m_eyePos;

    std::vector<std::shared_ptr<Object>> m_objects;
    std::vector<std::shared_ptr<Light>> m_lights;

public:
    Scene(int width = 1280, int height = 960);

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
    virtual std::optional<HitPayload> trace(const Ray &ray, const std::vector<std::shared_ptr<Object>> &objects) const;

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
    int getWidth() const { return m_width; }
    int getHeight() const { return m_height; }
    float getFov() const { return m_fov; }
    int getMaxDepth() const { return m_maxDepth; }
    const cv::Vec3f &getEyePos() const { return m_eyePos; }
    float getRussianRoulette() const { return m_russianRoulette; }

    void setCamera(const Camera &camera);

protected:
    std::pair<HitPayload, float> sampleLight() const;
};

class BVHScene : public Scene
{
private:
    std::shared_ptr<BVH> m_bvh;

    std::optional<HitPayload> intersect(const Ray &ray) const;

public:
    BVHScene(int width = 1280, int height = 960);

    void buildBVH();

    virtual cv::Vec3f castRay(const cv::Vec3f &eyePos, const cv::Vec3f &dir, int depth) const;

    virtual cv::Vec3f pathTracing(const cv::Vec3f &eyePos, const cv::Vec3f &dir, int depth) const override;

    friend void testSphereBVH();
};

#endif