#pragma once

#include <glm/glm.hpp>

#include "RendererProperties.h"

/*!
 *  @brief  Class containing all the properties required by a renderer to render it.
 */
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
    RendererProperties*     mRendProp;          /// Renderer specific properties for the object.
    double                  mShininess;         /// The exponential used in computing the specular highlights in the Phong lighting model.
    glm::vec3               mColor;             /// The color of the object; only used in the absence of a texture.
    glm::vec3               mSpecularColor;     /// The color of the specular highlight.
};

