#pragma once

#include <vector>

#include "Object.h"

class SceneReader
{
public:
    SceneReader();
    ~SceneReader();

    void read(std::string filename, std::vector<Object*>& objList);
};

