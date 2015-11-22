#pragma once

#include <vector>

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include "Camera.h"
#include "Object.h"
#include "PointLight.h"

struct GLFWwindow;

class Renderer
{
public:
	Renderer(std::vector<Object*>& objList);
	~Renderer();

    void setCamera(Camera& cam);
    void setLight(PointLight& light);
    bool init();
    void render();
    void close();

private:
    void initObjects();
    void load_shader(const char* vert_shader, const char* frag_shader);
    GLuint read_shader(const char* filename, GLenum shader_type);
    void render_obj(Object* obj, glm::mat4& cam);

private:
    std::vector<Object*>&   mObjList;
    GLFWwindow*             mWindow;
    GLuint                  mShader;
    PointLight              mLight;
    Camera                  mCamera;
};

