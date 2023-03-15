#include "common/BVH.h"
#include "common/Camera.h"
#include "objects/Sphere.h"
#include "objects/Triangle.h"
#include "Scene.h"
#include "Renderer.h"

void testSingle();
void testSphereBVH();
void testTriangleBVH();

int main()
{
    testSingle();
    testSphereBVH();
    testTriangleBVH();
    return 0;
}

void testSingle()
{
    std::shared_ptr<Object> sph1 = std::make_shared<Sphere>(cv::Vec3f(0, 0, 0), 1.0);
    std::shared_ptr<Object> sph2 = std::make_shared<Sphere>(cv::Vec3f(1, 1, 1), 1.0);
    std::vector<std::shared_ptr<Object>> objects = { sph1, sph2 };
    BVH bvh(objects);
    std::shared_ptr<BVH::BVHNode> res = bvh.getRoot();
    std::cout << "res = " << res->aabb << std::endl;
    if (res->left != nullptr)
        std::cout << "left = " << res->left->aabb << std::endl;
    if (res->right != nullptr)
        std::cout << "right = " << res->right->aabb << std::endl;
}

void testSphereBVH()
{
    Camera camera(1280, 960, 90.0f);
    BVHScene scene(camera, cv::Vec3f(0.843137, 0.67451, 0.235294));
    std::shared_ptr<Object> sph = std::make_shared<Sphere>(cv::Vec3f(0, 0, 0), 1);
    sph->setMaterialType(Material::MaterialType::DIFFUSE_AND_GLOSSY);
    sph->setDiffuseColor(cv::Vec3f(0.8, 0.7, 0.6));

    scene.add(std::move(sph));

    scene.buildBVH();
    std::optional<HitPayload> res = scene.trace(Ray(cv::Vec3f(0, 0, 0), cv::Vec3f(0.132292, 0.0427083, -1)));
    if (res.has_value())
    {
        std::cout << "Hit!" << std::endl;
    }
    else
    {
        std::cout << "Miss!" << std::endl;
    }
}

void testTriangleBVH()
{
    std::optional<std::vector<Triangle>> triangles = Triangle::loadModel("models/bunny/bunny.obj");
    if (!triangles.has_value())
    {
        std::cout << "Failed to load model" << std::endl;
        return;
    }

    Camera camera(800, 600, 90.0f);
    BVHScene scene(camera, cv::Vec3f(0.843137, 0.67451, 0.235294));
    for (const auto &tri : triangles.value())
    {
        scene.add(std::make_shared<Triangle>(tri));
    }
    scene.buildBVH();
}