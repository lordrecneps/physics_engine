#pragma once

#include "PhysicsObject.h"
#include "RenderObject.h"

#include <memory>

class Object
{
public:
    Object(double mass = 0.0);
    virtual ~Object() = 0;

    virtual void print() = 0;

	PhysicsObject& phyProp();
    RenderObject& rendProp();

private:
	PhysicsObject	mPhysicsObj;	//!< Physics
	RenderObject	mRenderObj;	    //!< Render
};

