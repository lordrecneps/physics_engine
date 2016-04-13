#include <iostream>

#include "GLFWWindow.h"

GLFWWindow::GLFWWindow()
    : mWindow(NULL)
    , mWidth(0)
    , mHeight(0)
    , mExit(false)
{
}

GLFWWindow::~GLFWWindow()
{
}

bool GLFWWindow::init(uint32_t width, uint32_t height)
{
    mWidth = width;
    mHeight = height;

    if(!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW." << std::endl;
        return false;
    }

    mWindow = glfwCreateWindow(mWidth, mHeight, "Boing", NULL, NULL);
    if(!mWindow)
    {
        std::cerr << "Could not open window with GLFW3" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(mWindow);
    glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    glfwSetCursorPos(mWindow, 0.5*mWidth, 0.5*mHeight);

    return true;
}

bool GLFWWindow::update()
{
    if(mExit)
        return false;

    if(!glfwWindowShouldClose(mWindow))
    {
        glfwSwapBuffers(mWindow);
        glm::dvec3 forward, up, right;
        glm::dmat4 cam = mCamera.get_matrix(forward, up, right);

        updateCamera(forward, up, right);
    }
    else
    {
        mExit = true;
        return false;
    }

    return true;
}

void GLFWWindow::updateCamera(glm::dvec3& forward, glm::dvec3& up, glm::dvec3& right)
{
    glfwPollEvents();

    if(glfwGetKey(mWindow, 'S')) {
        mCamera.mPos -= 0.1 * forward;
    }
    else if(glfwGetKey(mWindow, 'W')) {
        mCamera.mPos += 0.1 * forward;
    }
    if(glfwGetKey(mWindow, 'A')) {
        mCamera.mPos -= 0.1 * right;
    }
    else if(glfwGetKey(mWindow, 'D')) {
        mCamera.mPos += 0.1 * right;
    }
    if(glfwGetKey(mWindow, 'X')) {
        mCamera.mPos -= 0.1 * up;
    }
    else if(glfwGetKey(mWindow, 'Z')) {
        mCamera.mPos += 0.1 * up;
    }
    if(glfwGetKey(mWindow, GLFW_KEY_ESCAPE))
        glfwSetWindowShouldClose(mWindow, GL_TRUE);

    double mouseX, mouseY;
    glfwGetCursorPos(mWindow, &mouseX, &mouseY);
    mCamera.update_angles(0.1f * (float)(mouseY - 0.5*mHeight), 0.1f * (float)(mouseX - 0.5*mWidth));
    glfwSetCursorPos(mWindow, 0.5*mWidth, 0.5*mHeight);
}

void GLFWWindow::close()
{
    glfwTerminate();
}

Camera& GLFWWindow::getCamera()
{
    return mCamera;
}
