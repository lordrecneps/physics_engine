#include "Object.h"

Object::Object(double mass)
{
	physicsObject.setMass(mass);
}

Object::~Object()
{
}

PhysicsObject Object::getPhysicsObject()
{
	return physicsObject;
}
