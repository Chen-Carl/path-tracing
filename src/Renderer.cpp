#include <omp.h>
#include <iomanip>
#include <optional>
#include <chrono>
#include "Renderer.h"
#include "common/utils.h"

cv::Mat3f Renderer::render(const Scene &scene, const std::string &ckpt) const
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

// cv::Mat3f RayTracer::render(const Scene &scene) const
// {
//     int width = scene.getWidth();
//     int height = scene.getHeight();
//     assert(height % m_thread == 0);
//     int chunk = height / m_thread;
//     cv::Mat3f frameBuffer(height, width, cv::Vec3f(0.0f, 0.0f, 0.0f));

//     float scale = std::tan(zoe::deg2rad(scene.getFov()) / 2.0f);
//     cv::Vec3f eyePos = scene.getEyePos();
//     float imgAspectRatio = width / static_cast<float>(height);

//     indicators::ProgressBar bar = zoe::createProgressBar("Rendering", 40);

//     auto castRay = [&](int threadId) {
//         int end = chunk * (threadId + 1);
//         for (int j = end - chunk; j < end; j++)
//         {
//             for (int i = 0; i < width; i++)
//             {
//                 float x = (2 * (i + 0.5) / width - 1) * scale * imgAspectRatio;
//                 float y = (1 - 2 * (j + 0.5) / height) * scale;
//                 cv::Vec3f dir = cv::normalize(cv::Vec3f(-x, y, 1.0f));
//                 for (int s = 0; s < m_spp; s++)
//                 {
//                     frameBuffer(j, i) += scene.pathTracing(eyePos, dir) / m_spp;
//                 }
//                 bar.set_progress(((j - end + chunk) * width + i) / (float)(width * chunk) * 100.0f);
//             }
//         }
//     };

//     auto start = std::chrono::high_resolution_clock::now();

//     std::vector<std::thread> threads;
//     for (int i = 0; i < m_thread; i++)
//     {
//         threads.emplace_back(castRay, i);
//     }
//     for (auto &t : threads)
//     {
//         t.join();
//     }

//     auto end = std::chrono::high_resolution_clock::now();

//     std::cout << "Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms" << std::endl;

//     return frameBuffer;
// }


cv::Mat3f RayTracer::render(const Scene &scene, const std::string &ckpt) const
{
    int width = scene.getWidth();
    int height = scene.getHeight();
    cv::Vec3f eyePos = scene.getEyePos();
    cv::Mat3f frameBuffer(height, width, cv::Vec3f(0.0f, 0.0f, 0.0f));

    auto ckptFrameBuffer = getCkptFrameBuffer(ckpt);
    int spp = 0;

    if (ckptFrameBuffer.has_value())
    {
        auto & ckptFb = ckptFrameBuffer.value().first;
        spp = ckptFrameBuffer.value().second;
        assert(ckptFb.rows == height && ckptFb.cols == width);
        frameBuffer = ckptFb;
    }

    auto start = std::chrono::high_resolution_clock::now();

    int count = 0;
    int total = width * height;

#if ENABLE_OPENMP
    #pragma omp parallel for
#endif
    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            cv::Vec3f dir = scene.getRay(i, j);
            for (int s = 0; s < m_spp; s++)
            {
                frameBuffer(j, i) += scene.pathTracing(eyePos, dir) / (m_spp + spp);
            }
#if ENABLE_OPENMP
            #pragma omp critical
#endif
            std::cout << "\r" << ++count << "/" << total << " (" << std::fixed << std::setprecision(3) << (count / (float)total * 100.0f) << "%)" << std::flush;
            // std::cout << "frameBuffer(" << j << ", " << i << ") = " << frameBuffer(j, i) << std::endl;
        }
    }

    auto end = std::chrono::high_resolution_clock::now();

    std::cout << std::endl;
    std::cout << "Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms" << std::endl;

    return frameBuffer;
}

std::optional<std::pair<cv::Mat3f, int>> RayTracer::getCkptFrameBuffer(const std::string &ckpt) const
{
    cv::Mat3f res;
    if (!ckpt.empty())
    {
        std::cout << "Loading checkpoint: " << ckpt << std::endl;
        cv::Mat3f ckptImg = cv::imread(ckpt, cv::IMREAD_COLOR);
        if (size_t pos = ckpt.find_last_of("-"); pos != std::string::npos)
        {
            int spp = std::stoi(ckpt.substr(pos + 1));
            res = ckptImg * spp / (spp + m_spp);
            return std::make_pair(res / 255, spp);
        }
        else
        {
            std::cout << "Warning: invalid checkpoint file name, use default value" << std::endl;
            return std::nullopt;
        }
    }
    else
    {
        std::cout << "No checkpoint given, use default value" << std::endl;
    }
    return std::nullopt;
}
