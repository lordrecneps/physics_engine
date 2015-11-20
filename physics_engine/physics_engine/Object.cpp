#include "Object.h"

Object::Object(double mass)
{
	mPhysicsObj.setMass(mass);
}

Object::~Object()
{
}

PhysicsObject & Object::phyProp()
{
    return mPhysicsObj;
}

RenderObject & Object::rendProp()
{
    return mRenderObj;
}


