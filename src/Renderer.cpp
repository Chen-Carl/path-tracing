#include <optional>
#include "Renderer.h"
#include "common/utils.h"

cv::Mat3f Renderer::render(const Scene &scene) const
{
    // camera definition
    int width = scene.getWidth();
    int height = scene.getHeight();
    cv::Mat3f frameBuffer(height, width);
    float scale = std::tan(zoe::deg2rad(scene.getFov()) / 2.0f);
    cv::Vec3f eyePos = scene.getEyePos();
    float imgAspectRatio = width / static_cast<float>(height);

    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            // primary ray direction
            float x = (2 * (i + 0.5) / width - 1) * scale * imgAspectRatio;
            float y = (1 - 2 * (j + 0.5) / height) * scale;
            cv::Vec3f dir = cv::normalize(cv::Vec3f(x, y, -1.0f));
            frameBuffer(j, i) = scene.castRay(eyePos, dir, 0);
        }
    }

    return frameBuffer;
}

RayTracer::RayTracer(int numSamples) :
    m_numSamples(numSamples)
{

}

cv::Mat3f RayTracer::render(const Scene &scene) const
{
    int width = scene.getWidth();
    int height = scene.getHeight();
    cv::Mat3f frameBuffer(height, width);
    float scale = std::tan(zoe::deg2rad(scene.getFov()) / 2.0f);
    cv::Vec3f eyePos = scene.getEyePos();
    float imgAspectRatio = width / static_cast<float>(height);

    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            float x = (2 * (i + 0.5) / width - 1) * scale * imgAspectRatio;
            float y = (1 - 2 * (j + 0.5) / height) * scale;
            cv::Vec3f dir = cv::normalize(cv::Vec3f(-x, y, 1.0f));
            for (int s = 0; s < m_numSamples; s++)
            {
                frameBuffer(j, i) += scene.pathTracing(eyePos, dir, 0) / m_numSamples;
            }
        }
        zoe::updateProgress(j / static_cast<float>(height));
    }
    zoe::updateProgress(1.f);

    return frameBuffer;
}
