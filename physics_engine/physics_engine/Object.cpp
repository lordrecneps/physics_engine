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

void Object::setPos(glm::vec3 & pos)
{
    mPos = pos;
}

void Object::setRot(glm::vec3 & rot)
{
    mRot = rot;
}

void Object::setPose(glm::vec3 & pos, glm::vec3 & rot)
{
    mPos = pos;
    mRot = rot;
}
