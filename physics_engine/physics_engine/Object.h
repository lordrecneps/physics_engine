#pragma once

#include "PhysicsObject.h"
#include "RenderObject.h"

#include <memory>

class Object
{
public:
    Object();
    virtual ~Object() = 0;

private:
	std::shared_ptr<PhysicsObject>	physicsObject;	//!< Physics
	std::shared_ptr<RenderObject>	renderObject;	//!< Render
};

