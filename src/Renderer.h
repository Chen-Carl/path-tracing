#ifndef __RENDERER_H__
#define __RENDERER_H__

#include <opencv2/opencv.hpp>
#include "objects/Object.h"
#include "Scene.h"

class Renderer
{
public:
    Renderer() = default;
    virtual ~Renderer() = default;

    virtual cv::Mat3f render(const Scene &scene, const std::string &ckpt = "") const;
};

class RayTracer : public Renderer
{
public:
    int m_spp;
    int m_thread;

    std::optional<std::pair<cv::Mat3f, int>> getCkptFrameBuffer(const std::string &ckpt) const;

public:
    RayTracer(int spp = 32, int thread = 1);
    virtual ~RayTracer() = default;

    virtual cv::Mat3f render(const Scene &scene, const std::string &ckpt = "") const override;

};

#endif