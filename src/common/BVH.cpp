#include <numeric>
#include "BVH.h"
#include "objects/Object.h"

BVH::BVH(const std::vector<std::shared_ptr<Object>> &objects)
{
    m_objects = objects;
    m_root = init();
}

std::shared_ptr<BVH::BVHNode> BVH::init()
{
    return init(m_objects.begin(), m_objects.end());
}

std::shared_ptr<BVH::BVHNode> BVH::init(obj_iter begin, obj_iter end)
{
    std::shared_ptr<BVH::BVHNode> node = std::make_shared<BVH::BVHNode>();
    AABB bound = std::accumulate(begin, end, AABB{}, [](const AABB &acc, const auto obj) { 
        return acc + obj->getAABB(); 
    });

    if (end - begin == 1)
    {
        node->obj = *begin;
        node->aabb = (*begin)->getAABB();
        return node;
    }
    else
    {
        AABB centroidBound = std::accumulate(begin, end, AABB{}, [](const AABB &acc, const auto obj) { 
            return acc + obj->getAABB().getCentroid(); 
        });

        int axis = centroidBound.getLargestAxis();
        std::sort(begin, end, [axis](const auto &a, const auto &b) {
            return a->getAABB().getCentroid()[axis] < b->getAABB().getCentroid()[axis];
        });

        obj_iter middle = begin + (end - begin) / 2;
        node->left = init(begin, middle);
        node->right = init(middle, end);
        node->aabb = node->left->aabb + node->right->aabb;
    }
    return node;
}

std::optional<HitPayload> BVH::intersect(const Ray &ray)
{
    if (!m_root)
    {
        return std::nullopt;
    }
    return intersect(m_root, ray);
}

std::optional<HitPayload> BVH::intersect(const std::shared_ptr<BVHNode> &node, const Ray &ray)
{
    if (!node || !node->aabb.intersect(ray))
    {
        return std::nullopt;
    }
    if (node->left == nullptr && node->right == nullptr)
    {
        return node->obj->intersect(ray);
    }
    std::optional<HitPayload> left = intersect(node->left, ray);
    std::optional<HitPayload> right = intersect(node->right, ray);
    if (left && right)
    {
        return left->dist < right->dist ? left : right;
    }
    return left ? left : right;
}