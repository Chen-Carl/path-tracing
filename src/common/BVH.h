#ifndef __COMMON_BVH_H__
#define __COMMON_BVH_H__

#include <memory>
#include "AABB.h"
#include "objects/HitPayload.h"

class Object;

class BVH
{
public:
    class BVHNode
    {
    public:
        std::shared_ptr<Object> obj = nullptr;
        std::shared_ptr<BVHNode> left = nullptr;
        std::shared_ptr<BVHNode> right = nullptr;
        AABB aabb;
    };
private:
    using obj_iter = std::vector<std::shared_ptr<Object>>::iterator;

    std::shared_ptr<BVHNode> m_root = nullptr;
    std::vector<std::shared_ptr<Object>> m_objects;

    std::shared_ptr<BVH::BVHNode> init();
    std::shared_ptr<BVH::BVHNode> init(obj_iter begin, obj_iter end);
    
public:
    BVH() = default;
    BVH(const std::vector<std::shared_ptr<Object>> &objects);

    std::shared_ptr<BVHNode> getRoot() const { return m_root; }

    std::optional<HitPayload> intersect(const cv::Vec3f &orig, const cv::Vec3f &dir);

    static std::optional<HitPayload> intersect(const std::shared_ptr<BVHNode> &node, const cv::Vec3f &orig, const cv::Vec3f &dir);
};

#endif