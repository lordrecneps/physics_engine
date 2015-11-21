#include "RenderObject.h"

RenderObject::RenderObject()
    : mRendProp(nullptr)
{
}

RenderObject::~RenderObject()
{
}

RendererProperties * RenderObject::rendProp()
{
    return mRendProp;
}

void RenderObject::setRendProp(RendererProperties * prop)
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

void RenderObject::setSpecColor(glm::vec3 & specColor)
{
    mSpecularColor = specColor;
}

glm::vec3 RenderObject::getSpecColor()
{
    return mSpecularColor;
}
