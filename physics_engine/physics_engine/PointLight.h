#pragma once

#include <glm/glm.hpp>

class PointLight
{
public:
    PointLight(const glm::vec3& pos_ = glm::vec3(), const glm::vec3& color_ = glm::vec3(1.0), float amb_ = 0.05f, float atten_ = 0.01f) : pos(pos_), color(color_), ambient(amb_), attenuation(atten_) {}

    PointLight& operator=(const PointLight& light);

    glm::vec3 pos;
    glm::vec3 color;
    float ambient;
    float attenuation;
};