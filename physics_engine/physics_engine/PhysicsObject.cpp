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

void PhysicsObject::setPos(glm::vec3& pos)
{
	mPos = pos;
}

void PhysicsObject::setRot(glm::quat& rot)
{
	mRot = rot;
}

void PhysicsObject::setPose(glm::vec3& pos, glm::quat& rot)
{
	mPos = pos;
	mRot = rot;
}

double PhysicsObject::mass()
{
    return mMass;
}

glm::vec3 PhysicsObject::pos()
{
    return mPos;
}

glm::quat PhysicsObject::rot()
{
    return mRot;
}

glm::vec3 PhysicsObject::vel()
{
    return mVel;
}

glm::vec3 PhysicsObject::angVel()
{
    return mAngVel;
}

glm::vec3 PhysicsObject::force()
{
    return mForce;
}

glm::vec3 PhysicsObject::torque()
{
    return mTorque;
}
