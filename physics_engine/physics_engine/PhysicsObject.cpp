#include "PhysicsObject.h"

PhysicsObject::PhysicsObject()
{
}

PhysicsObject::~PhysicsObject()
{
}

void PhysicsObject::update()
{
	float timeStep; // TODO: placeholder. need time step from update loop.

	// linear
	glm::vec3 acc = mForce * glm::vec3(mInvMass);
	//acc += glm::vec3(0.0, -9.81, 0.0); // gravity
	mVel = mVel + acc*glm::vec3(timeStep);
	mPos = mPos + mVel*glm::vec3(timeStep);

	// angular

}

void PhysicsObject::setMass(double mass)
{
	mMass = mass;
	(mass != 0) ? mInvMass = 1.0 / mass : mInvMass = 0.0;
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

double PhysicsObject::invMass()
{
	return mInvMass;
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
