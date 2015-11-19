#include "Object.h"




Object::Object(double mass)
    :mMass(mass)
{
}

Object::~Object()
{
}

void Object::setMass(double mass)
{
    mMass = mass;
}
