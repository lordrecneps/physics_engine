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

	PhysicsObject getPhysicsObject();

private:
	PhysicsObject	physicsObject;	//!< Physics
	RenderObject	renderObject;	//!< Render
};

