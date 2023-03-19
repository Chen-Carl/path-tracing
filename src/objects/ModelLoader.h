#ifndef __OBJECTS_MODEL_H__
#define __OBJECTS_MODEL_H__

#define TINYOBJLOADER_IMPLEMENTATION
#define TINYOBJLOADER_USE_MAPBOX_EARCUT

#include <sstream>
#include <tinyxml2.h>
#include <tinyobjloader/tiny_obj_loader.h>
#include "common/Camera.h"
#include "objects/Triangle.h"
#include "Scene.h"

class ModelLoader
{
public:
    static std::pair<Camera, std::map<const std::string, cv::Vec3f>> loadXML(const std::string &filepath, const std::string &colorFmt = "bgr");

    static std::map<std::string, cv::Mat3f> loadTexture(const std::string &folder);

    static std::pair<std::vector<Triangle>, Camera> loadOBJ(const std::string &filename, const std::string &colorFmt = "bgr");

    static BVHScene loadBVHScene(const std::string &filename);

};

#endif