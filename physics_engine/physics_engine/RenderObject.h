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

private:
    RendererProperties*     mRendProp;
    double                  mShininess;
    glm::vec3               mSpecularColor;
};

