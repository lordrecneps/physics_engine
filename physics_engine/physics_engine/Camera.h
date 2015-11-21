#pragma once
#include <glm/glm.hpp>

class Camera
{
public:
    Camera(double fov_ = 45.0, double zs_ = -1.5);
    ~Camera();

    glm::mat4x4 get_matrix(glm::vec3* forward = 0, glm::vec3* up = 0, glm::vec3* right = 0);

    void update_angles(float vAngle_, float hAngle_);

    void adjust_zoom(float delta);

    glm::vec3       mPos;
    double          mVAngle;
    double          mHAngle;
    double          mZoomSensitivity;
    double          mFov;
};


