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

#endif