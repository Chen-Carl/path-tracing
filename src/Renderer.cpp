#include <optional>
#include "Renderer.h"
#include "common/utils.h"

#define __OUTPUT_DEBUG_INFO__ true

cv::Vec3f Renderer::castRay(const cv::Vec3f &eyePos, const cv::Vec3f &dir, const Scene &scene, int depth) const
{
    if (depth > scene.getMaxDepth())
    {
        return cv::Vec3f(0.0f, 0.0f, 0.0f);
    }

    cv::Vec3f hitColor = scene.getBgColor();
    std::optional<HitPayload> payload = trace(eyePos, dir, scene.getObjects());
    if (payload.has_value())
    {
        auto [uv, hitObj, tNear] = payload.value();
        cv::Vec3f hitPoint = eyePos + dir * tNear;
        cv::Vec3f hitNormal = hitObj->getNormal(hitPoint);

        switch (hitObj->getMaterialType())
        {
        // only reflection
        case Object::MaterialType::REFLECTION:
            {
                float kr = zoe::fresnel(dir, hitNormal, hitObj->getIor());
                cv::Vec3f reflectionDir = cv::normalize(zoe::reflect(dir, hitNormal));
                cv::Vec3f reflectionRayOrig = (reflectionDir.dot(hitNormal) < 0) ? 
                        hitPoint - hitNormal * scene.getEpsilon() : 
                        hitPoint + hitNormal * scene.getEpsilon();
                hitColor = castRay(reflectionRayOrig, reflectionDir, scene, depth + 1) * kr;
                break;
            }
        // reflection + refraction
        case Object::MaterialType::REFLECTION_AND_REFRACTION:
            {
                cv::Vec3f reflectionDir = cv::normalize(zoe::reflect(dir, hitNormal));
                cv::Vec3f refractionDir = cv::normalize(zoe::refract(dir, hitNormal, hitObj->getIor()));
                cv::Vec3f reflectionRayOrig = (reflectionDir.dot(hitNormal) < 0) ? 
                        hitPoint - hitNormal * scene.getEpsilon() : 
                        hitPoint + hitNormal * scene.getEpsilon();
                cv::Vec3f refractionRayOrig = (refractionDir.dot(hitNormal) < 0) ? 
                        hitPoint - hitNormal * scene.getEpsilon() : 
                        hitPoint + hitNormal * scene.getEpsilon();
                cv::Vec3f reflectionColor = castRay(reflectionRayOrig, reflectionDir, scene, depth + 1);
                cv::Vec3f refractionColor = castRay(refractionRayOrig, refractionDir, scene, depth + 1);
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
                        hitPoint + hitNormal * scene.getEpsilon() : 
                        hitPoint - hitNormal * scene.getEpsilon();
                for (const auto &light : scene.getLights())
                {
                    cv::Vec3f lightDir = cv::normalize(light->getPos() - hitPoint);
                    std::optional<HitPayload> payload = trace(shadowOrig, lightDir, scene.getObjects());
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
                hitColor = lightAmt * hitObj->getDiffuseColor() * hitObj->getKd() + specularColor * hitObj->getKs();
                break;
            }
        }
    }
    return hitColor;
}

std::optional<HitPayload> Renderer::trace(const cv::Vec3f &orig, const cv::Vec3f &dir, const std::vector<std::shared_ptr<Object>> &objects) const
{
    float nearest = std::numeric_limits<float>::max();
    std::optional<HitPayload> hitPayload;
    for (const auto &obj : objects)
    {
        auto res = obj->intersect(orig, dir);
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

cv::Mat3f Renderer::render(const Scene &scene) const
{
    // camera definition
    int width = scene.getWidth();
    int height = scene.getHeight();
    cv::Mat3f frameBuffer(height, width);
    float scale = std::tan(zoe::deg2rad(scene.getFov()) / 2.0f);
    cv::Vec3f eyePos = cv::Vec3f(0.0f, 0.0f, 0.0f);
    float imgAspectRatio = width / static_cast<float>(height);

    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            // primary ray direction
            float x = (2 * (i + 0.5) / width - 1) * scale * imgAspectRatio;
            float y = (1 - 2 * (j + 0.5) / height) * scale;
            cv::Vec3f dir = cv::normalize(cv::Vec3f(x, y, -1.0f));
            frameBuffer(j, i) = castRay(eyePos, dir, scene, 0) * 255;
        }
    }
    return frameBuffer;
}