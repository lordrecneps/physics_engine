#include "PhysicsObject.h"

PhysicsObject::PhysicsObject()
{
}

PhysicsObject::~PhysicsObject()
{
}

void PhysicsObject::setMass(double mass)
{
	mMass = mass;
}

void PhysicsObject::setPos(glm::vec3 & pos)
{
	mPos = pos;
}

void PhysicsObject::setRot(glm::vec3 & rot)
{
	mRot = rot;
}

void PhysicsObject::setPose(glm::vec3 & pos, glm::vec3 & rot)
{
	mPos = pos;
	mRot = rot;
}
