#include <iostream>

#include "Camera.h"

#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>


Camera::Camera(double fov, double zs)
    : mPos()
    , mVAngle(0.0)
    , mHAngle(0.0)
    , mFov(fov)
    , mZoomSensitivity(zs)
    , mNearPlane(0.1)
    , mFarPlane(30.0)
{
}

glm::dmat4 Camera::get_matrix(glm::dvec3& forward, glm::dvec3& up, glm::dvec3& right)
{
    forward = mForward;
    up = mUp;
    right = mRight;
    return mCamMatrix;
}



void Camera::update_angles(float vAngle_, float hAngle_)
{
    mVAngle += vAngle_;
    mHAngle += hAngle_;

    mHAngle = fmodf(static_cast<float>(mHAngle), 360.0f);
    mVAngle = (mVAngle > 85.0f) ? 85.0f : mVAngle;
    mVAngle = (mVAngle < -85.0f) ? -85.0f : mVAngle;

    mCamMatrix = glm::perspective(glm::radians(mFov), 800 / 600.0, mNearPlane, mFarPlane);

    glm::dmat4 orientation;
    orientation = glm::rotate(orientation, glm::radians(mVAngle), glm::dvec3(1, 0, 0));
    orientation = glm::rotate(orientation, glm::radians(mHAngle), glm::dvec3(0, 1, 0));
    mCamMatrix *= orientation;
    mCamMatrix = glm::translate(mCamMatrix, -mPos);

    mForward = glm::dvec3(glm::inverse(orientation) * glm::vec4(0, 0, -1, 1));
    mUp = glm::dvec3(glm::inverse(orientation) * glm::vec4(0, 1, 0, 1));
    mRight = glm::dvec3(glm::inverse(orientation) * glm::vec4(1, 0, 0, 1));
}

void Camera::adjust_zoom(float delta)
{
    mFov += mZoomSensitivity * delta;
    if (mFov < 5.0f) mFov = 5.0f;
    if (mFov > 130.0f) mFov = 130.0f;
}

Camera& Camera::operator=(const Camera& cam)
{
    if (this == &cam)
        return *this;

    mPos = cam.mPos;
    mVAngle = cam.mVAngle;
    mHAngle = cam.mHAngle;
    mZoomSensitivity = cam.mZoomSensitivity;
    mFov = cam.mFov;
    mNearPlane = cam.mNearPlane;
    mFarPlane = cam.mFarPlane;

    return *this;
}


Camera::~Camera()
{
}

