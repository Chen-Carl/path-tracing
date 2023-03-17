#include "objects/ModelLoader.h"
#include "ModelLoader.h"

std::pair<Camera, std::map<const std::string, cv::Vec3f>> ModelLoader::loadXML(const std::string &filepath, const std::string &colorFmt)
{
    Camera camera;
    std::map<const std::string, cv::Vec3f> lights;

    tinyxml2::XMLDocument doc;
    doc.LoadFile(filepath.c_str());
    if (doc.Error())
    {
        std::cerr << "Error loading XML file: " << filepath << std::endl;
        return std::make_pair(camera, lights);
    }

    auto node = doc.RootElement();
    while (node)
    {
        if (std::string(node->Name()) == "camera")
        {
            int width = node->IntAttribute("width");
            int height = node->IntAttribute("height");
            float fov = node->FloatAttribute("fovy");

            camera.width = width;
            camera.height = height;
            camera.fov = fov;

            auto eye = node->FirstChildElement("eye");
            auto lookat = node->FirstChildElement("lookat");
            auto up = node->FirstChildElement("up");
            
            if (eye && lookat && up)
            {
                camera.eyePos = cv::Vec3f(
                    eye->FloatAttribute("x"),
                    eye->FloatAttribute("y"),
                    eye->FloatAttribute("z")
                );
                camera.lookat = cv::Vec3f(
                    lookat->FloatAttribute("x"),
                    lookat->FloatAttribute("y"),
                    lookat->FloatAttribute("z")
                );
                camera.up = cv::Vec3f(
                    up->FloatAttribute("x"),
                    up->FloatAttribute("y"),
                    up->FloatAttribute("z")
                );
            }
            else
            {
                std::cerr << "Error loading camera: lacking (eye, lookat, up) parameters" << std::endl;
                return std::make_pair(camera, lights);
            }
        }
        else if (std::string(node->Name()) == "light")
        {
            const std::string mtlname = node->Attribute("mtlname");
            const std::string radiance = node->Attribute("radiance");

            // input float with delimiter ','
            std::stringstream ss(radiance);
            std::string token;
            std::vector<float> radianceVec;
            while (std::getline(ss, token, ','))
            {
                radianceVec.push_back(std::stof(token));
            }
            if (colorFmt == "bgr")
            {
                lights[mtlname] = cv::Vec3f(
                    radianceVec[2], 
                    radianceVec[1],
                    radianceVec[0]
                );
            }
            else
            {
                lights[mtlname] = cv::Vec3f(
                    radianceVec[0],
                    radianceVec[1],
                    radianceVec[2]
                );
            }
        }
        node = node->NextSiblingElement();
    }
    camera.init();
    return std::make_pair(camera, lights);
}

std::pair<std::vector<Triangle>, Camera> ModelLoader::loadOBJ(const std::string &filename, const std::string &colorFmt)
{
    const std::string folder = filename.substr(0, filename.find_last_of("/\\") + 1);
    const std::string file = filename.substr(filename.find_last_of("/\\") + 1);
    const std::string modelName = file.substr(0, file.find_last_of("."));
    const std::string xmlName = folder + modelName + ".xml";

    auto [camera, lights] = ModelLoader::loadXML(xmlName);

    tinyobj::ObjReaderConfig readerConfig;
    readerConfig.mtl_search_path = "";
    tinyobj::ObjReader reader;

    if (!reader.ParseFromFile(filename, readerConfig))
    {
        if (!reader.Error().empty())
        {
            std::cerr << "TinyObjReader: " << reader.Error();
        }
        exit(1);
    }

    if (!reader.Warning().empty())
    {
        std::cout << "TinyObjReader: " << reader.Warning();
    }

    auto &attrib = reader.GetAttrib();
    auto &shapes = reader.GetShapes();
    auto &materials = reader.GetMaterials();

    std::vector<Triangle> triangles;

    for (size_t s = 0; s < shapes.size(); s++)
    {
        size_t index_offset = 0;
        // the number of meshes
        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
        {
            // fv = 3, create a triangle
            size_t fv = static_cast<size_t>(shapes[s].mesh.num_face_vertices[f]);

            std::array<cv::Vec3f, 3> vertices;
            std::array<cv::Vec3f, 3> vnormals;
            std::array<cv::Vec3f, 3> vcolors;
            std::array<cv::Vec2f, 3> vtexcoords;

            for (size_t v = 0; v < fv; v++)
            {
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                tinyobj::real_t vx = attrib.vertices[3 * size_t(idx.vertex_index) + 0];
                tinyobj::real_t vy = attrib.vertices[3 * size_t(idx.vertex_index) + 1];
                tinyobj::real_t vz = attrib.vertices[3 * size_t(idx.vertex_index) + 2];
                vertices[v] = cv::Vec3f(vx, vy, vz);

                if (idx.normal_index >= 0)
                {
                    tinyobj::real_t nx = attrib.normals[3 * size_t(idx.normal_index) + 0];
                    tinyobj::real_t ny = attrib.normals[3 * size_t(idx.normal_index) + 1];
                    tinyobj::real_t nz = attrib.normals[3 * size_t(idx.normal_index) + 2];
                    vnormals[v] = cv::Vec3f(nx, ny, nz);
                }

                if (idx.texcoord_index >= 0)
                {
                    tinyobj::real_t tx = attrib.texcoords[2 * size_t(idx.texcoord_index) + 0];
                    tinyobj::real_t ty = attrib.texcoords[2 * size_t(idx.texcoord_index) + 1];
                    vtexcoords[v] = cv::Vec2f(tx, ty);
                }

                tinyobj::real_t red = attrib.colors[3*size_t(idx.vertex_index)+0];
                tinyobj::real_t green = attrib.colors[3*size_t(idx.vertex_index)+1];
                tinyobj::real_t blue = attrib.colors[3*size_t(idx.vertex_index)+2];

                if (colorFmt == "bgr")
                {
                    vcolors[v] = cv::Vec3f(blue, green, red);
                }
                else
                {
                    vcolors[v] = cv::Vec3f(red, green, blue);
                }
            }
            index_offset += fv;

            Triangle triangle(vertices);
            triangle.setNormals(vnormals);
            triangle.setColors(vcolors);
            triangle.setTexCoords(vtexcoords);

            int materialId = shapes[s].mesh.material_ids[f];
            const std::string materialName = materials[materialId].name;
            Material material;
            material.materialType = Material::MaterialType::DIFFUSE_AND_GLOSSY;

            if (lights.count(materialName))
            {
                material.emission = lights[materialName];
            }
            else 
            {   
                if (colorFmt == "bgr")
                {
                    material.emission = cv::Vec3f(
                        materials[materialId].emission[2],
                        materials[materialId].emission[1], 
                        materials[materialId].emission[0]
                    );
                }
                else
                {
                    material.emission = cv::Vec3f(
                        materials[materialId].emission[0],
                        materials[materialId].emission[1], 
                        materials[materialId].emission[2]
                    );
                }
            }

            if (colorFmt == "bgr")
            {
                material.kd = cv::Vec3f(
                    materials[materialId].diffuse[2], 
                    materials[materialId].diffuse[1], 
                    materials[materialId].diffuse[0]
                );
                material.ks = cv::Vec3f(
                    materials[materialId].specular[2], 
                    materials[materialId].specular[1], 
                    materials[materialId].specular[0]
                );
                material.tr = cv::Vec3f(
                    materials[materialId].transmittance[2], 
                    materials[materialId].transmittance[1], 
                    materials[materialId].transmittance[0]
                );
            }
            else 
            {
                material.kd = cv::Vec3f(
                    materials[materialId].diffuse[0], 
                    materials[materialId].diffuse[1], 
                    materials[materialId].diffuse[2]
                );
                material.ks = cv::Vec3f(
                    materials[materialId].specular[0], 
                    materials[materialId].specular[1], 
                    materials[materialId].specular[2]
                );
                material.tr = cv::Vec3f(
                    materials[materialId].transmittance[0], 
                    materials[materialId].transmittance[1], 
                    materials[materialId].transmittance[2]
                );
            }
            material.ior = materials[materialId].ior;
            material.specularExp = materials[materialId].shininess;
            triangle.setMaterial(material);

            triangles.push_back(triangle);
        }
    }
    std::cout << "Loaded " << triangles.size() << " triangles from " << filename << std::endl;
    return std::make_pair(triangles, camera);
}

BVHScene ModelLoader::loadBVHScene(const std::string &filename)
{
    auto [triangles, camera] = ModelLoader::loadOBJ(filename);
    BVHScene scene(camera, cv::Vec3f());
    for (const auto &triangle : triangles)
    {
        scene.add(std::make_shared<Triangle>(triangle));
    }
    return scene;
}