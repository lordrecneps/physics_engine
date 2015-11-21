#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>


Camera::Camera(double fov, double zs)
    : mPos()
    , mVAngle(0.0)
    , mHAngle(0.0)
    , mFov(fov)
    , mZoomSensitivity(zs)
{
}

glm::mat4 Camera::get_matrix(glm::vec3* forward, glm::vec3* up, glm::vec3* right)
{
    glm::mat4 camera = glm::perspective(glm::radians(mFov), 800 / 600.0, 0.1, 30.0);

    glm::mat4 orientation;
    orientation = glm::rotate(orientation, (float)glm::radians(mVAngle), glm::vec3(1, 0, 0));
    orientation = glm::rotate(orientation, (float)glm::radians(mHAngle), glm::vec3(0, 1, 0));
    camera *= orientation;

    if (forward)
        *forward = glm::vec3(glm::inverse(orientation) * glm::vec4(0, 0, -1, 1));
    if (up)
        *up = glm::vec3(glm::inverse(orientation) * glm::vec4(0, 1, 0, 1));
    if (right)
        *right = glm::vec3(glm::inverse(orientation) * glm::vec4(1, 0, 0, 1));

    return glm::translate(camera, -mPos);
    return glm::mat4x4();
}

void Camera::update_angles(float vAngle_, float hAngle_)
{
    mVAngle += vAngle_;
    mHAngle += hAngle_;

    mHAngle = fmodf(static_cast<float>(mHAngle), 360.0f);
    mVAngle = (mVAngle > 85.0f) ? 85.0f : mVAngle;
    mVAngle = (mVAngle < -85.0f) ? -85.0f : mVAngle;
}

void Camera::adjust_zoom(float delta)
{
    mFov += mZoomSensitivity * delta;
    if (mFov < 5.0f) mFov = 5.0f;
    if (mFov > 130.0f) mFov = 130.0f;
}


Camera::~Camera()
{
}

