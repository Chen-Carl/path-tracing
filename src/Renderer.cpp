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

    indicators::ProgressBar bar = zoe::createProgressBar("Rendering", 40);

    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            // primary ray direction
            float x = (2 * (i + 0.5) / width - 1) * scale * imgAspectRatio;
            float y = (1 - 2 * (j + 0.5) / height) * scale;
            cv::Vec3f dir = cv::normalize(cv::Vec3f(x, y, -1.0f));
            frameBuffer(j, i) = scene.castRay(eyePos, dir, 0);

            bar.set_progress((j * width + i) / (float)(width * height) * 100.0f);
        }
    }

    return frameBuffer;
}

RayTracer::RayTracer(int spp, int thread) :
    m_spp(spp),
    m_thread(thread)
{

}

cv::Mat3f RayTracer::render(const Scene &scene) const
{
    int width = scene.getWidth();
    int height = scene.getHeight();
    assert(height % m_thread == 0);
    int chunk = height / m_thread;
    cv::Mat3f frameBuffer(height, width, cv::Vec3f(0.0f, 0.0f, 0.0f));

    float scale = std::tan(zoe::deg2rad(scene.getFov()) / 2.0f);
    cv::Vec3f eyePos = scene.getEyePos();
    float imgAspectRatio = width / static_cast<float>(height);

    indicators::ProgressBar bar = zoe::createProgressBar("Rendering", 40);

    auto castRay = [&](int threadId) {
        int end = chunk * (threadId + 1);
        for (int j = end - chunk; j < end; j++)
        {
            for (int i = 0; i < width; i++)
            {
                float x = (2 * (i + 0.5) / width - 1) * scale * imgAspectRatio;
                float y = (1 - 2 * (j + 0.5) / height) * scale;
                cv::Vec3f dir = cv::normalize(cv::Vec3f(-x, y, 1.0f));
                for (int s = 0; s < m_spp; s++)
                {
                    frameBuffer(j, i) += scene.pathTracing(eyePos, dir) / m_spp;
                }
                bar.set_progress(((j - end + chunk) * width + i) / (float)(width * chunk) * 100.0f);
            }
        }
    };

    std::vector<std::thread> threads;
    for (int i = 0; i < m_thread; i++)
    {
        threads.emplace_back(castRay, i);
    }
    for (auto &t : threads)
    {
        t.join();
    }

    return frameBuffer;
}
