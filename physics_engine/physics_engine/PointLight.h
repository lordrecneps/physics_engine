#pragma once

#include <glm/glm.hpp>

/*!
 *  @brief  A point light with a quadratic attenuation model.
 */
class PointLight
{
public:
    PointLight(const glm::dvec3& pos_ = glm::dvec3(), const glm::dvec3& color_ = glm::dvec3(1.0), float amb_ = 0.05f, float atten_ = 0.01f)
        : pos(pos_)
        , color(color_)
        , ambient(amb_)
        , attenuation(atten_) 
    {
    }

    PointLight& operator=(const PointLight& light);

    glm::dvec3 pos;      /// The position of the light.
    glm::dvec3 color;    /// The color of the light [0, 1]
    float ambient;      /// The total ambient light generated by this point light. (hack)
    float attenuation;  /// The rate at which the light from this point attenuates with distance.
};