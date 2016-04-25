#include <iostream>

#include "PhysicsObject.h"

#include "glm/gtx/euler_angles.hpp"
#include "glm/gtx/quaternion.hpp"
#include <glm/gtx/string_cast.hpp>

PhysicsObject::PhysicsObject()
    : mInvMass(0.0)
{
}

PhysicsObject::~PhysicsObject()
{
}

void PhysicsObject::update(double time_step)
{
	// linear
	glm::dvec3 linearAcc = mForce * mInvMass;

    if (mInvMass)
	    linearAcc += glm::dvec3(0.0, -0.0981, 0.0); // gravity

	mVel = mVel + linearAcc * time_step;
	mPos = mPos + mVel * time_step;

	// angular
	glm::dvec3 angularAcc = mInvInertia * mTorque;
	mAngVel = mAngVel + angularAcc * time_step;

    mRot = mRot + 0.5 * mRot * glm::dquat(0.0, mAngVel) * time_step;
	mRot = glm::normalize(mRot);
}

void PhysicsObject::setMass(double mass)
{
	(mass != 0) ? mInvMass = 1.0f / mass : mInvMass = 0.0f;
}

void PhysicsObject::setPos(glm::dvec3& pos)
{
	mPos = pos;
}

void PhysicsObject::setRot(glm::dquat& rot)
{
	mRot = rot;
}

void PhysicsObject::setPose(glm::dvec3& pos, glm::dquat& rot)
{
	mPos = pos;
	mRot = rot;
}

void PhysicsObject::setVel(glm::dvec3 & vel)
{
    mVel = vel;
}

void PhysicsObject::setAngVel(glm::dvec3& angVel)
{
    mAngVel = angVel;
}

void PhysicsObject::setCollisionShape(CollisionShape* shape)
{
	mColShape = shape;
}

double PhysicsObject::invMass() const
{
	return mInvMass;
}

glm::dvec3 PhysicsObject::pos() const
{
    return mPos;
}

glm::dquat PhysicsObject::rot() const
{
    return mRot;
}

glm::dvec3 PhysicsObject::vel() const
{
    return mVel;
}

glm::dvec3 PhysicsObject::angVel() const
{
    return mAngVel;
}

glm::dvec3 PhysicsObject::force() const
{
    return mForce;
}

glm::dvec3 PhysicsObject::torque() const
{
    return mTorque;
}

CollisionShape* PhysicsObject::collisionShape() const
{
	return mColShape;
}

