#pragma once
#include <glm/glm.hpp>

/*
 *  @brief  An FPS-style camera.
 */
class Camera
{
public:
    Camera(double fov_ = 45.0, double zs_ = -1.5);
    ~Camera();

    /*
     *  @brief Calculates the projection matrix for the camera. Also calculates the forward, up and right vectors of the camera,
     *         based on it's orientation.
     *  @param[out] forward The direction in which the camera is looking. (0, 0, -1) by default.
     *  @param[out] up      The up vector of the camera. (0, 1, 0) by default.
     *  @param[out] right   The right vector of the camera. (1, 0, 0) by default.
     */
    glm::dmat4x4 get_matrix(glm::dvec3& forward, glm::dvec3& up, glm::dvec3& right);

    /*
     *  @brief Adds the given angular deviations to the horizontal and vertical orientation of the camera.
     *  @param[in]  The delta for the vertical angle.
     *  @param[in]  The delta for the horizontal angle.
     */
    void update_angles(float vAngle_, float hAngle_);

    /*!
     *  @brief  Increases the zoom by a factor of delta, modulated by the zoom sensitivity.
     *  @param[in]  The amount to zoom in by. Zooms out if negative.
     */
    void adjust_zoom(float delta);

    /*!
     *  @brief  Copies every field.
     *  @param[in]  cam Reference to the camera whose properties are to be copied.
     *  @return Reference to the camera that was assigned to.
     */
    Camera& operator=(const Camera& cam);

    glm::dvec3       mPos;               /// Position of the camera
    double          mVAngle;            /// The vertical angle of the camera. 0 means the forward vector lies on the x-z plane.
    double          mHAngle;            /// The horizontal angle of the camera. 0 means the forward vector lies on the y-z plane.
    double          mZoomSensitivity;   /// How quickly the camera zoom changes.
    double          mFov;               /// The field of view of the camera.
    double          mNearPlane;         /// The near plane clipping distance.
    double          mFarPlane;          /// The far plane clipping distance.

    glm::dmat4       mCamMatrix;
    glm::dvec3       mForward;
    glm::dvec3       mUp;
    glm::dvec3       mRight;
};


