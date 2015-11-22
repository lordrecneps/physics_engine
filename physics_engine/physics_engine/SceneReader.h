#pragma once

#include <vector>

#include <pugixml.hpp>

#include "Camera.h"
#include "Object.h"
#include "PointLight.h"

class SceneReader
{
public:
    SceneReader();
    ~SceneReader();

    bool read(std::string filename);
    void readObjects(std::vector<Object*>& objList);
    void readCamera(Camera& cam);
    void readLight(PointLight& light);

private:
    pugi::xml_document      mDoc;
    pugi::xml_node          mRoot;
};

