#ifndef __OBJECTS_HITPAYLOAD_H__
#define __OBJECTS_HITPAYLOAD_H__

#include <memory>
#include <utility>
#include <opencv2/opencv.hpp>

class Object;

struct HitPayload
{
    cv::Vec2f uv;                         // interpolated coefficients
    std::shared_ptr<const Object> hitObj; // hit object
    float dist;                           // t in light equation: r(t) = o + t * d

    HitPayload(cv::Vec2f uv = 0, std::shared_ptr<const Object> hitObj = nullptr, float dist = 0)
        : uv(uv), hitObj(hitObj), dist(dist)
    {
    }
};

#endif