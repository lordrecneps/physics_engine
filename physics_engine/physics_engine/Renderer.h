#pragma once

#include <vector>
#include "Object.h"

struct GLFWwindow;

class Renderer
{
public:
	Renderer(std::vector<Object*>& objList);
	~Renderer();

    bool init();
    void render();
    void close();

private:
    std::vector<Object*>&   mObjList;
    GLFWwindow*             mWindow;
};

