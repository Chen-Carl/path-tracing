#include "objects/Sphere.h"
#include "Scene.h"
#include "Renderer.h"

void testRenderer(const Scene &scene, const Renderer &renderer);

int main()
{
    Scene scene(1280, 900);
    std::shared_ptr<Object> sph1 = std::make_shared<Sphere>(cv::Vec3f(-1, 0, -12), 2);
    sph1->setMaterialType(Object::MaterialType::DIFFUSE_AND_GLOSSY);
    sph1->setDiffuseColor(cv::Vec3f(0.8, 0.7, 0.6));

    std::shared_ptr<Object> sph2 = std::make_shared<Sphere>(cv::Vec3f(0.5, -0.5, -8), 1.5);
    sph2->setIor(1.5);
    sph2->setMaterialType(Object::MaterialType::DIFFUSE_AND_GLOSSY);

    scene.add(std::move(sph1));
    scene.add(std::move(sph2));

    scene.add(std::make_shared<Light>(cv::Vec3f(-20, 70, 20), cv::Vec3f(0.5, 0.5, 0.5)));
    scene.add(std::make_shared<Light>(cv::Vec3f(30, 50, -12), cv::Vec3f(0.5, 0.5, 0.5)));

    Renderer renderer;
    testTrace(scene, renderer);
    testCastRay(scene, renderer);
    testRenderer(scene, renderer);

    return 0;
}

void testTrace(const Scene &scene, const Renderer &renderer)
{
    std::cout << "========== testTrace ==========" << std::endl;
    std::optional<HitPayload> res;
#define XX(x, y, z, dx, dy, dz, dist) \
    res = renderer.trace(cv::Vec3f(x, y, z), cv::Vec3f(dx, dy, dz), scene.getObjects()); \
    if (res.has_value()) \
    { \
        auto [uv, hitObj, tNear] = res.value(); \
        std::cout << "Hit: (expected " << #dist << ") " << tNear << std::endl; \
    } \
    else \
    { \
        std::cout << "Hit: (expected " << #dist << ") No hit" << std::endl; \
    }

    XX(0, 0, 0, -0.046875, 0.065625, -1, 7.2272)
    XX(-0.354775, 0.475551, -7.24659, 0.258281, -0.288285, -0.922054, 2.3934)
    XX(0.182199, -0.142952, -6.57821, -0.197291, 0.241293, -0.950186, 2.40424)
    XX(-0.444263, 0.494633, -8.60754, 0.0519302, -0.0513969, -0.997327, 1.54643)
    XX(-0.437215, 0.483238, -7.36373, 0.303832, -0.316533, -0.898606, 2.33549)
    XX(0.277902, -0.267359, -9.4651, 0.544518, -0.564125, 0.620696, 2.32313)
    XX(0, 0, 0, -0.178125, 0.065625, -1, 10.3145)
    XX(-0.53762, -1.56339, -8.20618, 0.421233, 0.514588, 0.746835, 2.27656)
    XX(0.361122, -0.363258, -6.51273, -0.403193, 0.382023, -0.831561, 2.25707)
#undef XX
}

void testCastRay(const Scene &scene, const Renderer &renderer)
{
    std::cout << "========== testCastRay ==========" << std::endl;
    std::optional<HitPayload> res;
    cv::Vec3f color;
#define XX(x, y, z, dx, dy, dz, depth, r, b, g) \
    color = renderer.castRay(cv::Vec3f(x, y, z), cv::Vec3f(dx, dy, dz), scene, depth); \
    std::cout << "orig = " << cv::Vec3f(x, y, z) << ", dir = " << cv::Vec3f(dx, dy, dz) << ", hitColor = " << color << ", depth = " << depth << " (expected " << cv::Vec3f(r, b, g) << ")" << std::endl;

    XX(0.674275, -0.211139, -6.53844, 0.328201, 0.340772, 0.887601, 1, 0, 0, 0)
    XX(1.5818, -0.225963, -6.99771, 0.963233, 0.154453, -0.316961, 5, 0, 0, 0)
    XX(0, 0, 0, 0.359375, -0.0322917, -1, 0, 0.235294, 0.67451, 0.843137)
    XX(0, 0, 0, 0.00104167, 0.0427083, -1, 0, 0.118196, 0.118196, 0.118196)
    XX(0, 0, 0, 0.132292, 0.0427083, -1, 0, 0.134982, 0.134982, 0.134982)
    XX(0, 0, 0, 0.219792, 0.0427083, -1, 0, 0.0976897, 0.0976897, 0.0976897)
}

void testRenderer(const Scene &scene, const Renderer &renderer)
{
    std::cout << "========== testRenderer ==========" << std::endl;
    cv::Mat img = renderer.render(scene);
    cv::imwrite("testRender.png", img);
}