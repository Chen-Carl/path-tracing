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
    cv::Vec3f emission;                   // emission intensity
    cv::Vec3f point;                      // sample point

    HitPayload(cv::Vec2f uv = 0, std::shared_ptr<const Object> hitObj = nullptr, float dist = 0, cv::Vec3f emission = cv::Vec3f(0, 0, 0))
        : uv(uv), hitObj(hitObj), dist(dist), emission(emission)
    {

    }

    bool emissive() const
    {
        return emission != cv::Vec3f(0, 0, 0);
    }
};

template <size_t I>
auto get(const HitPayload &payload)
{
    if constexpr (I == 0)
    {
        return payload.uv;
    }
    else if constexpr (I == 1)
    {
        return payload.hitObj;
    }
    else if constexpr (I == 2)
    {
        return payload.dist;
    }
    else if constexpr (I == 3)
    {
        return payload.emission;
    }
    else if constexpr (I == 4)
    {
        return payload.point;
    }
}

namespace std
{
    template <>
    struct tuple_size<HitPayload> : public integral_constant<size_t, 4>
    {

    };

    template <size_t I>
    struct tuple_element<I, HitPayload>
    {
        using type = decltype(get<I>(std::declval<HitPayload>()));
    };
}

#endif