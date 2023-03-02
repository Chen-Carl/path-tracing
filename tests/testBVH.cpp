#include "common/BVH.h"
#include "objects/Sphere.h"
#include "Scene.h"
#include "Renderer.h"

void testSingle();

int main()
{
    testSingle();
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

void testBVHRender()
{
    BVHScene scene(1280, 900);
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

    scene.buildBVH();

    Renderer renderer;
    
    cv::Mat img = renderer.render(scene);
    cv::imwrite("testRender.png", img);
}