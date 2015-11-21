#pragma once

#include <vector>
#include "Object.h"

class Renderer
{
public:
	Renderer(std::vector<Object*>& objList);
	~Renderer();

private:
    std::vector<Object*>&   mObjList;
};

