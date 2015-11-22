#include "RenderObject.h"

RenderObject::RenderObject()
    : mRendProp(nullptr)
    , mShininess(100.0)
    , mSpecularColor(glm::vec3(1.0))
{
}

RenderObject::~RenderObject()
{
}

RendererProperties* RenderObject::rendProp()
{
    return mRendProp;
}

void RenderObject::setRendProp(RendererProperties* prop)
{
    mRendProp = prop;
}

void RenderObject::setShininess(double sh)
{
    mShininess = sh;
}

double RenderObject::getShininess()
{
    return mShininess;
}

void RenderObject::setSpecColor(glm::vec3& specColor)
{
    mSpecularColor = specColor;
}

glm::vec3 RenderObject::getSpecColor()
{
    return mSpecularColor;
}

void RenderObject::setColor(glm::vec3& col)
{
    mColor = col;
}

glm::vec3 RenderObject::getColor()
{
    return mColor;
}
