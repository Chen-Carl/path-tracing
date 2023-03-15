#include "common/Camera.h"
#include "objects/Sphere.h"
#include "objects/Triangle.h"
#include "Scene.h"
#include "Renderer.h"

int main()
{
    Camera camera(640, 480, 90.0f);
    BVHScene scene(camera, cv::Vec3f(0.843137, 0.67451, 0.235294));
    std::shared_ptr<Object> sph1 = std::make_shared<Sphere>(cv::Vec3f(-1, 0, -12), 2);
    sph1->setMaterialType(Material::MaterialType::DIFFUSE_AND_GLOSSY);
    sph1->setDiffuseColor(cv::Vec3f(0.8, 0.7, 0.6));

    std::shared_ptr<Object> sph2 = std::make_shared<Sphere>(cv::Vec3f(0.5, -0.5, -8), 1.5);
    sph2->setIor(1.5);
    sph2->setMaterialType(Material::MaterialType::REFLECTION_AND_REFRACTION);

    scene.add(std::move(sph1));
    scene.add(std::move(sph2));

    std::vector<cv::Vec3f> vertices = {
        cv::Vec3f(-5, -3, -6),
        cv::Vec3f(5, -3, -6),
        cv::Vec3f(5, -3, -16),
        cv::Vec3f(-5, -3, -16)
    };
    std::vector<cv::Vec2f> stCoord = {
        cv::Vec2f(0, 0),
        cv::Vec2f(1, 0),
        cv::Vec2f(1, 1),
        cv::Vec2f(0, 1)
    };

    std::shared_ptr<Triangle> tri1 = std::make_shared<Triangle>(std::array<cv::Vec3f, 3>({vertices[0], vertices[1], vertices[3]}));
    tri1->setMaterialType(Material::MaterialType::DIFFUSE_AND_GLOSSY);
    tri1->setStCoords({stCoord[0], stCoord[1], stCoord[3]});

    std::shared_ptr<Triangle> tri2 = std::make_shared<Triangle>(std::array<cv::Vec3f, 3>({vertices[1], vertices[2], vertices[3]}));
    tri2->setMaterialType(Material::MaterialType::DIFFUSE_AND_GLOSSY);
    tri2->setStCoords({stCoord[1], stCoord[2], stCoord[3]});

    scene.add(std::move(tri1));
    scene.add(std::move(tri2));

    scene.add(std::make_shared<Light>(cv::Vec3f(-20, 70, 20), cv::Vec3f(0.5, 0.5, 0.5)));
    scene.add(std::make_shared<Light>(cv::Vec3f(30, 50, -12), cv::Vec3f(0.5, 0.5, 0.5)));

    scene.buildBVH();

    Renderer renderer;
    cv::Mat3f res = renderer.render(scene);
    cv::imwrite("output/testBVHRender.png", res * 255);

    // cv::Vec3f color = scene.castRay(cv::Vec3f(0, 0, 0), cv::Vec3f(0.109375, -0.338542, -1), 0);
    // std::cout << color << std::endl;
    
    return 0;
}
