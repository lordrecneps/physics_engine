#pragma once

#include "ObjectTypes.h"
#include "PhysicsObject.h"
#include "RenderObject.h"

class Object
{
public:
    Object(ObjectType type, double mass = 0.0);
    virtual ~Object() = 0;

    virtual void print() = 0;

	PhysicsObject& phyProp();
    RenderObject& rendProp();

    ObjectType type();

protected:
	PhysicsObject	mPhysicsObj;	//!< Physics properties.
	RenderObject	mRenderObj;	    //!< Render properties.
    ObjectType      mType;          //!< The type of the object.
};

