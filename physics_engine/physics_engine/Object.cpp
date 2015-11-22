#include "Object.h"

Object::Object(ObjectType type, double mass)
    : mType(type)
{
	mPhysicsObj.setMass(mass);
}

Object::~Object()
{
}

PhysicsObject& Object::phys()
{
    return mPhysicsObj;
}

RenderObject & Object::rend()
{
    return mRenderObj;
}

ObjectType Object::type()
{
    return mType;
}


