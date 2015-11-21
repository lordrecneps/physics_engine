#include "Object.h"

Object::Object(ObjectType type, double mass)
    : mType(type)
{
	mPhysicsObj.setMass(mass);
}

Object::~Object()
{
}

PhysicsObject& Object::phyProp()
{
    return mPhysicsObj;
}

RenderObject & Object::rendProp()
{
    return mRenderObj;
}

ObjectType Object::type()
{
    return mType;
}


