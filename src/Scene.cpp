#include <opencv2/opencv.hpp>
#include <numeric>
#include "common/utils.h"
#include "Scene.h"

Scene::Scene(const Camera &camera, const cv::Vec3f &bgColor) : 
    m_camera(camera),
    m_bgColor(bgColor)
{
    m_objects = std::vector<std::shared_ptr<Object>>();
    m_lights = std::vector<std::shared_ptr<Light>>();
}

std::pair<HitPayload, float> Scene::sampleLight() const
{
    float prob = zoe::randomFloat() * m_totalLightArea;

    float emitArea = 0;
    for (const auto &obj : m_objects) 
    {
        if (obj->emissive()) 
        {
            emitArea += obj->getArea();
            if (prob <= emitArea) 
            {
                return std::make_pair(obj->samplePoint(), 1 / m_totalLightArea);
            }
        }
    }
    throw std::runtime_error("No light found");
}

void Scene::add(std::shared_ptr<Object> object)
{
    m_objects.push_back(object);
    if (object->emissive())
    {
        m_totalLightArea += object->getArea();
    }
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
    std::optional<HitPayload> payload = trace(Ray(eyePos, dir));
    if (payload.has_value())
    {
        auto [uv, hitObj, tNear, emission] = payload.value();
        cv::Vec3f hitPoint = eyePos + dir * tNear;
        cv::Vec3f hitNormal = hitObj->getNormal(hitPoint);
        cv::Vec2f st = hitObj->getStCoords(uv);
        switch (hitObj->getMaterialType())
        {
        // only reflection
        case Material::MaterialType::REFLECTION:
            {
                float kr = zoe::fresnel(dir, hitNormal, hitObj->getIor());
                cv::Vec3f reflectionDir = cv::normalize(zoe::reflect(dir, hitNormal));
                cv::Vec3f reflectionRayOrig = (reflectionDir.dot(hitNormal) < 0) ? 
                        hitPoint - hitNormal * m_epsilon : 
                        hitPoint + hitNormal * m_epsilon;
                hitColor = castRay(reflectionRayOrig, reflectionDir, depth + 1) * kr;
                break;
            }
        // reflection + refraction
        case Material::MaterialType::REFLECTION_AND_REFRACTION:
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
                    std::optional<HitPayload> payload = trace(Ray(shadowOrig, lightDir));
                    if (payload.has_value())
                    {
                        auto [uv, hitObj, tNear, emission] = payload.value();
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
                    cv::Vec3f reflectionDir = cv::normalize(zoe::reflect(-lightDir, hitNormal));
                    specularColor += std::pow(std::max(0.0f, -reflectionDir.dot(dir)), hitObj->getSpecularExp()) * light->getIntensity();
                }
                hitColor = lightAmt.mul(hitObj->getDiffuseColor(st)).mul(hitObj->getKd()) + specularColor.mul(hitObj->getKs());
                break;
            }
        }
    }
    return hitColor;
}

std::optional<HitPayload> Scene::trace(const Ray &ray) const
{
    float nearest = std::numeric_limits<float>::max();
    std::optional<HitPayload> hitPayload;
    for (const auto &obj : m_objects)
    {
        auto res = obj->intersect(ray);
        if (res.has_value())
        {
            if (res->dist > zoe::selfCrossEpsilon && res->dist < nearest)
            {
                nearest = res->dist;
                hitPayload = res.value();
            }
        }
    }
    return hitPayload;
}

cv::Vec3f Scene::pathTracing(const cv::Vec3f &eyePos, const cv::Vec3f &dir) const
{
    cv::Vec3f directLight;
    cv::Vec3f indirectLight;
    std::optional<HitPayload> payload = trace(Ray(eyePos, dir));
    if (payload.has_value())
    {
        if (payload->emission != cv::Vec3f(0, 0, 0))
        {
            return payload->emission;
        }

        auto [uv, hitObj, tNear, emission] = payload.value();
        cv::Vec3f hitPoint = payload->point;
        cv::Vec3f hitNormal = cv::normalize(hitObj->getNormal(hitPoint));

        // sample the light
        auto [light, lightPdf] = sampleLight();
        cv::Vec3f lightPos = light.point;
        // from light to object
        cv::Vec3f lightDir = cv::normalize(hitPoint - lightPos);
        cv::Vec3f lightNormal = cv::normalize(light.hitObj->getNormal(lightPos));
        float dis = cv::norm(lightPos - hitPoint);

        switch (hitObj->getMaterialType())
        {
            case Material::MaterialType::DIFFUSE_AND_GLOSSY:
            {
                directLight = calDirectLight(lightPos, lightDir, lightNormal, lightPdf, dir, hitNormal, dis);
                indirectLight = calIndirectLight(hitObj, hitNormal, hitPoint, dir);
                return directLight + indirectLight;
            }
            case Material::MaterialType::REFLECTION:
            {
                return calIndirectLight(hitObj, hitNormal, hitPoint, dir, true);
            }
            case Material::MaterialType::REFLECTION_AND_REFRACTION:
            {
                return calIndirectLight(hitObj, hitNormal, hitPoint, dir, true);
            }
            case Material::MaterialType::DIFFUSE_AND_REFLECTION:
            {
                directLight = calDirectLight(lightPos, lightDir, lightNormal, lightPdf, dir, hitNormal, dis);
                indirectLight = calIndirectLight(hitObj, hitNormal, hitPoint, dir);
                return directLight + indirectLight;
            }
        }
    }
    return cv::Vec3f(0, 0, 0);
}

cv::Vec3f Scene::calDirectLight(const cv::Vec3f &lightPos, const cv::Vec3f &lightDir, const cv::Vec3f &lightNormal, float lightPdf, const cv::Vec3f &dir, const cv::Vec3f &hitNormal, float dis) const
{
    std::optional<HitPayload> shadowPayload = trace(Ray(lightPos, lightDir));
    // if the light is not occluded
    if (shadowPayload.has_value() && std::abs(shadowPayload->dist - dis) <= zoe::selfCrossEpsilon)
    {
        cv::Vec3f lightColor = light.emission;
        cv::Vec3f contri = shadowPayload->hitObj->evalLightBRDF(hitNormal, dir, -lightDir);
        float cosTheta = -lightDir.dot(hitNormal);
        float cosPhi = lightDir.dot(lightNormal);
#if OUTPUT_DEBUG_LOG
        std::cout << "========== direct light ==========" << std::endl;
        std::cout << "lightColor = " << lightColor << std::endl;
        std::cout << "contri = " << contri << std::endl;
        std::cout << "cosTheta = " << cosTheta << std::endl;
        std::cout << "cosPhi = " << cosPhi << std::endl;
        std::cout << "lightPdf = " << lightPdf << std::endl;
        std::cout << "dis = " << dis << std::endl;
        std::cout << "directLight = " << lightColor.mul(contri) * cosTheta * cosPhi / (lightPdf * dis * dis) << std::endl;
        std::cout << std::endl;
#endif
        cv::Vec3f res = lightColor.mul(contri) * cosTheta * cosPhi / (lightPdf * dis * dis);
        float specularExp = shadowPayload->hitObj->getSpecularExp();
        if (specularExp > zoe::denominatorEpsilon)
        {
            cv::Vec3f specular = std::pow(std::max(0.0f, -lightDir.dot(dir)), specularExp) * lightColor.mul(shadowPayload->hitObj->getKs()) / specularExp;
            return res + specular;
        }
        return res;
    }
    return cv::Vec3f(0, 0, 0);
}

cv::Vec3f Scene::calIndirectLight(const std::shared_ptr<const Object> &hitObj, const cv::Vec3f &hitNormal, const cv::Vec3f &hitPoint, const cv::Vec3f &dir, bool addDirectLight) const
{
    // indirect light
    if (zoe::randomFloat() < getRussianRoulette())
    {
        cv::Vec3f wi = cv::normalize(hitObj->getMaterial().sampleDir(hitNormal, dir));
        std::optional<HitPayload> indirectPayload = trace(Ray(hitPoint, wi));
        if (!addDirectLight)
        {
            if (indirectPayload.has_value() 
                && !indirectPayload->emissive())
            {
                cv::Vec3f contri = hitObj->evalLightBRDF(hitNormal, dir, wi);
                float cosTheta = wi.dot(hitNormal);
                float pdf = hitObj->getMaterial().pdf(hitNormal, dir, wi);
                if (pdf > zoe::denominatorEpsilon)
                {
                    cv::Vec3f res = pathTracing(hitPoint, wi).mul(contri) * cosTheta / (pdf * m_russianRoulette);
                    return res;
                }
            }
        }
        else
        {
            if (indirectPayload.has_value())
            {
                cv::Vec3f contri = hitObj->evalLightBRDF(hitNormal, dir, wi);
                float cosTheta = wi.dot(hitNormal);
                float pdf = hitObj->getMaterial().pdf(hitNormal, dir, wi);
                if (pdf > zoe::denominatorEpsilon)
                {
                    return pathTracing(hitPoint, wi).mul(contri) * std::abs(cosTheta) / (pdf * m_russianRoulette);
                }
            }
        }
    }
    return cv::Vec3f(0, 0, 0);
}

cv::Vec3f Scene::getRay(int x, int y) const
{
    return m_camera.getRayDir(x, y);
}

std::optional<HitPayload> BVHScene::trace(const Ray &ray) const
{
    return m_bvh->intersect(ray);
}

BVHScene::BVHScene(const Camera &camera, const cv::Vec3f &bgColor) : Scene(camera, bgColor)
{

}

void BVHScene::buildBVH()
{
    std::cout << "Building BVH..." << std::endl;
    m_bvh = std::make_shared<BVH>(getObjects());
}
