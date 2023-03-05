#include <opencv2/opencv.hpp>
#include "common/utils.h"
#include "Scene.h"

Scene::Scene(int width, int height) : 
    m_width(width), 
    m_height(height)
{
    m_objects = std::vector<std::shared_ptr<Object>>();
    m_lights = std::vector<std::shared_ptr<Light>>();
}

void Scene::add(std::shared_ptr<Object> object)
{
    m_objects.push_back(object);
}

void Scene::add(std::shared_ptr<Light> light)
{
    m_lights.push_back(light);
}

cv::Vec3f Scene::castRay(const cv::Vec3f &eyePos, const cv::Vec3f &dir, int depth) const
{
    if (depth > getMaxDepth())
    {
        return cv::Vec3f(0.0f, 0.0f, 0.0f);
    }

    cv::Vec3f hitColor = m_bgColor;
    std::optional<HitPayload> payload = trace(Ray(eyePos, dir), m_objects);
    if (payload.has_value())
    {
        auto [uv, hitObj, tNear] = payload.value();
        cv::Vec3f hitPoint = eyePos + dir * tNear;
        cv::Vec3f hitNormal = hitObj->getNormal(hitPoint);
        cv::Vec2f st = hitObj->getStCoords(uv);
        switch (hitObj->getMaterialType())
        {
        // only reflection
        case Object::MaterialType::REFLECTION:
            {
                float kr = zoe::fresnel(dir, hitNormal, hitObj->getIor());
                cv::Vec3f reflectionDir = zoe::reflect(dir, hitNormal);
                cv::Vec3f reflectionRayOrig = (reflectionDir.dot(hitNormal) < 0) ? 
                        hitPoint - hitNormal * m_epsilon : 
                        hitPoint + hitNormal * m_epsilon;
                hitColor = castRay(reflectionRayOrig, reflectionDir, depth + 1) * kr;
                break;
            }
        // reflection + refraction
        case Object::MaterialType::REFLECTION_AND_REFRACTION:
            {
                cv::Vec3f reflectionDir = cv::normalize(zoe::reflect(dir, hitNormal));
                cv::Vec3f refractionDir = cv::normalize(zoe::refract(dir, hitNormal, hitObj->getIor()));
                cv::Vec3f reflectionRayOrig = (reflectionDir.dot(hitNormal) < 0) ? 
                        hitPoint - hitNormal * m_epsilon : 
                        hitPoint + hitNormal * m_epsilon;
                cv::Vec3f refractionRayOrig = (refractionDir.dot(hitNormal) < 0) ? 
                        hitPoint - hitNormal * m_epsilon : 
                        hitPoint + hitNormal * m_epsilon;
                cv::Vec3f reflectionColor = castRay(reflectionRayOrig, reflectionDir, depth + 1);
                cv::Vec3f refractionColor = castRay(refractionRayOrig, refractionDir, depth + 1);
                float kr = zoe::fresnel(dir, hitNormal, hitObj->getIor());
                hitColor = reflectionColor * kr + refractionColor * (1 - kr);
                break;
            }
        default:
            {
                // Blinn-Phong shading model
                cv::Vec3f lightAmt = cv::Vec3f(0.0f, 0.0f, 0.0f);
                cv::Vec3f specularColor = cv::Vec3f(0.0f, 0.0f, 0.0f);
                cv::Vec3f shadowOrig = (dir.dot(hitNormal) < 0) ? 
                        hitPoint + hitNormal * m_epsilon : 
                        hitPoint - hitNormal * m_epsilon;
                for (const auto &light : m_lights)
                {
                    cv::Vec3f lightDir = cv::normalize(light->getPos() - hitPoint);
                    std::optional<HitPayload> payload = trace(Ray(shadowOrig, lightDir), m_objects);
                    if (payload.has_value())
                    {
                        auto [uv, hitObj, tNear] = payload.value();
                        if (cv::norm(tNear) >= cv::norm(light->getPos() - hitPoint))
                        {
                            // is shadow
                            lightAmt += light->getIntensity() * std::max(0.0f, hitNormal.dot(lightDir));
                        }
                    }
                    else
                    {
                        lightAmt += light->getIntensity() * std::max(0.0f, hitNormal.dot(lightDir));
                    }
                    cv::Vec3f reflectionDir = zoe::reflect(-lightDir, hitNormal);
                    specularColor += std::pow(std::max(0.0f, -reflectionDir.dot(dir)), hitObj->getSpecularExp()) * light->getIntensity();
                }
                hitColor = lightAmt.mul(hitObj->getDiffuseColor(st)) * hitObj->getKd() + specularColor * hitObj->getKs();
                break;
            }
        }
    }
    return hitColor;
}

std::optional<HitPayload> Scene::trace(const Ray &ray, const std::vector<std::shared_ptr<Object>> &objects) const
{
    float nearest = std::numeric_limits<float>::max();
    std::optional<HitPayload> hitPayload;
    for (const auto &obj : objects)
    {
        auto res = obj->intersect(ray);
        if (res.has_value())
        {
            if (res->dist < nearest)
            {
                nearest = res->dist;
                hitPayload = res.value();
            }
        }
    }
    return hitPayload;
}

void Scene::setCamera(const Camera &camera)
{
    m_bgColor = camera.bgColor;
    m_eyePos = camera.eyePos;
    m_fov = camera.fov;
}

std::optional<HitPayload> BVHScene::intersect(const Ray &ray) const
{
    return m_bvh->intersect(ray);
}

BVHScene::BVHScene(int width, int height) : Scene(width, height)
{
    
}

void BVHScene::buildBVH()
{
    std::cout << "Building BVH..." << std::endl;
    m_bvh = std::make_shared<BVH>(getObjects());
}

cv::Vec3f BVHScene::castRay(const cv::Vec3f &eyePos, const cv::Vec3f &dir, int depth) const
{
    if (depth > getMaxDepth())
    {
        return cv::Vec3f(0.0f, 0.0f, 0.0f);
    }
    cv::Vec3f hitColor = getBgColor();
    std::optional<HitPayload> intersection = intersect(Ray(eyePos, dir));
    if (intersection.has_value())
    {
        auto [uv, hitObj, tNear] = intersection.value();
        cv::Vec3f hitPoint = eyePos + dir * tNear;
        cv::Vec3f hitNormal = hitObj->getNormal(hitPoint);
        cv::Vec2f st = hitObj->getStCoords(uv);
        switch (hitObj->getMaterialType())
        {
        case Object::MaterialType::REFLECTION:
            {
                float kr = zoe::fresnel(dir, hitNormal, hitObj->getIor());
                cv::Vec3f reflectionDir = cv::normalize(zoe::reflect(dir, hitNormal));
                cv::Vec3f reflectionRayOrig = (reflectionDir.dot(hitNormal) < 0) ? 
                        hitPoint - hitNormal * getEpsilon() : 
                        hitPoint + hitNormal * getEpsilon();
                hitColor = castRay(reflectionRayOrig, reflectionDir, depth + 1) * kr;
                break;
            }
        // reflection + refraction
        case Object::MaterialType::REFLECTION_AND_REFRACTION:
            {
                cv::Vec3f reflectionDir = cv::normalize(zoe::reflect(dir, hitNormal));
                cv::Vec3f refractionDir = cv::normalize(zoe::refract(dir, hitNormal, hitObj->getIor()));
                cv::Vec3f reflectionRayOrig = (reflectionDir.dot(hitNormal) < 0) ? 
                        hitPoint - hitNormal * getEpsilon() : 
                        hitPoint + hitNormal * getEpsilon();
                cv::Vec3f refractionRayOrig = (refractionDir.dot(hitNormal) < 0) ? 
                        hitPoint - hitNormal * getEpsilon() : 
                        hitPoint + hitNormal * getEpsilon();
                cv::Vec3f reflectionColor = castRay(reflectionRayOrig, reflectionDir, depth + 1);
                cv::Vec3f refractionColor = castRay(refractionRayOrig, refractionDir, depth + 1);
                float kr = zoe::fresnel(dir, hitNormal, hitObj->getIor());
                hitColor = reflectionColor * kr + refractionColor * (1 - kr);
                break;
            }
        default:
            {
                // Blinn-Phong shading model
                cv::Vec3f lightAmt = cv::Vec3f(0.0f, 0.0f, 0.0f);
                cv::Vec3f specularColor = cv::Vec3f(0.0f, 0.0f, 0.0f);
                cv::Vec3f shadowOrig = (dir.dot(hitNormal) < 0) ? 
                        hitPoint + hitNormal * getEpsilon() : 
                        hitPoint - hitNormal * getEpsilon();
                for (const auto &light : getLights())
                {
                    cv::Vec3f lightDir = cv::normalize(light->getPos() - hitPoint);
                    std::optional<HitPayload> payload = intersect(Ray(shadowOrig, lightDir));
                    if (payload.has_value())
                    {
                        auto [uv, hitObj, tNear] = payload.value();
                        if (cv::norm(tNear) >= cv::norm(light->getPos() - hitPoint))
                        {
                            // is shadow
                            lightAmt += light->getIntensity() * std::max(0.0f, hitNormal.dot(lightDir));
                        }
                    }
                    else
                    {
                        lightAmt += light->getIntensity() * std::max(0.0f, hitNormal.dot(lightDir));
                    }
                    cv::Vec3f reflectionDir = zoe::reflect(-lightDir, hitNormal);
                    specularColor += std::pow(std::max(0.0f, -reflectionDir.dot(dir)), hitObj->getSpecularExp()) * light->getIntensity();
                }
                hitColor = lightAmt.mul(hitObj->getDiffuseColor(st)) * hitObj->getKd() + specularColor * hitObj->getKs();
                break;
            }
        }
    }
    return hitColor;
}
