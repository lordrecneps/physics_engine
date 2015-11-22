#include "PointLight.h"

PointLight& PointLight::operator=(const PointLight& light)
{
    if(this == &light)
        return *this;

    pos = light.pos;
    color = light.color;
    ambient = light.ambient;
    attenuation = light.attenuation;

    return *this;
}
