#ifndef __RENDERER_H__
#define __RENDERER_H__

#include <opencv2/opencv.hpp>
#include "objects/Object.h"
#include "Scene.h"

class Renderer
{
private:
    /**
     * @brief Cast a ray into the scene and return the color of the first object hit.
     * @param eyePos The position of the camera.
     * @param dir The direction of the ray (pixel - camera).
     * @param scene The scene to cast the ray into.
     * @param depth The number of bounces of the ray.
     * @return The color of the first object hit.
    */
    virtual cv::Vec3f castRay(const cv::Vec3f &eyePos, const cv::Vec3f &dir, const Scene &scene, int depth) const;

    /**
     * @brief Trace a ray into the scene and return the closest object hit.
     * @param orig The origin of the ray.
     * @param dir The direction of the ray.
     * @param objects The objects to trace the ray into.
     * @return The closest object hit.
    */
    std::optional<HitPayload> trace(const cv::Vec3f &orig, const cv::Vec3f &dir, const std::vector<std::shared_ptr<Object>> &objects) const;

public:
    Renderer() = default;
    virtual ~Renderer() = default;

    virtual cv::Mat3f render(const Scene &scene) const;

    friend void testTrace(const Scene &scene, const Renderer &renderer);
    friend void testCastRay(const Scene &scene, const Renderer &renderer);
};

#endif