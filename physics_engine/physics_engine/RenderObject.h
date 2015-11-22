#pragma once

#include <glm/glm.hpp>

#include "RendererProperties.h"

class RenderObject
{
public:
	RenderObject();
	~RenderObject();

    RendererProperties* rendProp();
    void setRendProp(RendererProperties* prop);

    void setShininess(double sh);
    double getShininess();

    void setSpecColor(glm::vec3& specColor);
    glm::vec3 getSpecColor();

    void setColor(glm::vec3& col);
    glm::vec3 getColor();

private:
    RendererProperties*     mRendProp;
    double                  mShininess;
    glm::vec3               mColor;
    glm::vec3               mSpecularColor;
};

