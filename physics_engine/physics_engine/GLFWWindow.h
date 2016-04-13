#pragma once

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include "Window.h"

class GLFWWindow : public Window
{
public:
    GLFWWindow();
    ~GLFWWindow();

    bool init(uint32_t width, uint32_t height);

    bool update();

    /*!
    *  @brief  Closes the GLFW window and cleans up the OpenGL context.
    */
    void close();

    Camera& getCamera();

private:
    void updateCamera(glm::dvec3& forward, glm::dvec3& up, glm::dvec3& right);

private:
    GLFWwindow*             mWindow;                /// The GLFW window to render to.
    GLuint                  mWidth;                 /// Width of the window.
    GLuint                  mHeight;                /// Height of the window.
    Camera                  mCamera;
    bool                    mExit;
};

