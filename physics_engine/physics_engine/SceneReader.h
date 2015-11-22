#pragma once

#include <vector>

#include <pugixml.hpp>

#include "Camera.h"
#include "Object.h"

class SceneReader
{
public:
    SceneReader();
    ~SceneReader();

    void read(std::string filename);
    void readObjects(std::vector<Object*>& objList);
    void readCamera(Camera& cam);

private:
    pugi::xml_document      mDoc;
    pugi::xml_node          mRoot;
};

