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

    virtual cv::Mat3f render(const Scene &scene) const;
};

class RayTracer : public Renderer
{
private:
    int m_numSamples;

public:
    RayTracer(int numSamples = 64);
    virtual ~RayTracer() = default;

    virtual cv::Mat3f render(const Scene &scene) const override;
};

#endif