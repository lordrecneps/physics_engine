#pragma once

#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

#include "CollisionShape.h"

/*!
 *  @brief  Class containing all of the physical properties used by the physics engine.
 */
class PhysicsObject
{
public:
	PhysicsObject();
	~PhysicsObject();

    PhysicsObject(const PhysicsObject&) = delete;
    PhysicsObject(PhysicsObject&&) = delete;

	/*!
	 *  @brief	Update physics object
	 */
	void update();
	
	// Accessors/Mutators
	// Setters
	void setMass(double mass);
	void setPos(glm::dvec3& pos);
	void setRot(glm::dquat& rot);
	void setPose(glm::dvec3& pos, glm::dquat& rot);
    void setVel(glm::dvec3& vel);
    void setAngVel(glm::dvec3& angVel);

	void setCollisionShape(CollisionShape* shape);

	// Getters
    double invMass() const;
    glm::dvec3 pos() const;
    glm::dquat rot() const;
    glm::dvec3 vel() const;
    glm::dvec3 angVel() const;
    glm::dvec3 force() const;
    glm::dvec3 torque() const;

	CollisionShape* collisionShape() const;

private:
    double      mInvMass;       /// Inverse mass of the object.
	glm::dvec3  mPos;           /// The position of the object.
	glm::dquat  mRot;           /// The orientation of the object.
    glm::dvec3  mVel;           /// The velocity of the object.
    glm::dvec3  mAngVel;        /// The angular velocity of the object.
	glm::dvec3  mForce;         /// The net force acting on the object.
	glm::dvec3  mTorque;        /// The net torque acting on the object.
	glm::dvec3	mInvInertia;	/// The inverse inertia of the object.

	CollisionShape*	mColShape;	/// The collision shape of the object
};

