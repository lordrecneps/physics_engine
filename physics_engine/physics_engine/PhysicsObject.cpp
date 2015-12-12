#include "PhysicsObject.h"

#include "glm\gtx\euler_angles.hpp"
#include "glm\gtx\quaternion.hpp"

PhysicsObject::PhysicsObject()
{
}

PhysicsObject::~PhysicsObject()
{
}

void PhysicsObject::update()
{
	float timeStep = 0.1f; // TODO: placeholder. need time step from update loop.

	// linear
	glm::vec3 linearAcc = mForce * mInvMass;
	//acc += glm::vec3(0.0, -9.81, 0.0); // gravity
	mVel = mVel + linearAcc * timeStep;
	mPos = mPos + mVel * timeStep;

	// angular
	glm::vec3 angularAcc = mInvInertia * mTorque;
	mAngVel = mAngVel + angularAcc * timeStep;
	mRot = mRot + glm::toQuat(glm::orientate3(0.5f * mRot * mAngVel * timeStep));
	glm::normalize(mRot);
}

void PhysicsObject::setMass(float mass)
{
	(mass != 0) ? mInvMass = 1.0f / mass : mInvMass = 0.0f;
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

void PhysicsObject::setVel(glm::vec3 & vel)
{
    mVel = vel;
}

void PhysicsObject::setCollisionShape(CollisionShape* shape)
{
	mColShape = shape;
}

float PhysicsObject::invMass() const
{
	return mInvMass;
}

glm::vec3 PhysicsObject::pos() const
{
    return mPos;
}

glm::quat PhysicsObject::rot() const
{
    return mRot;
}

glm::vec3 PhysicsObject::vel() const
{
    return mVel;
}

glm::vec3 PhysicsObject::angVel() const
{
    return mAngVel;
}

glm::vec3 PhysicsObject::force() const
{
    return mForce;
}

glm::vec3 PhysicsObject::torque() const
{
    return mTorque;
}

CollisionShape* PhysicsObject::collisionShape() const
{
	return mColShape;
}

