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

	/*!
	 *  @brief	Update physics object
	 */
	void update();
	
	// Accessors/Mutators
	// Setters
	void setMass(float mass);
	void setPos(glm::vec3& pos);
	void setRot(glm::quat& rot);
	void setPose(glm::vec3& pos, glm::quat& rot);
    void setVel(glm::vec3& vel);

	void setCollisionShape(CollisionShape* shape);

	// Getters
    float invMass() const;
    glm::vec3 pos() const;
    glm::quat rot() const;
    glm::vec3 vel() const;
    glm::vec3 angVel() const;
    glm::vec3 force() const;
    glm::vec3 torque() const;

	CollisionShape* collisionShape() const;

private:
    float       mInvMass;       /// Inverse mass of the object.
	glm::vec3   mPos;           /// The position of the object.
	glm::quat   mRot;           /// The orientation of the object.
    glm::vec3   mVel;           /// The velocity of the object.
    glm::vec3   mAngVel;        /// The angular velocity of the object.
	glm::vec3   mForce;         /// The net force acting on the object.
	glm::vec3   mTorque;        /// The net torque acting on the object.
	glm::vec3	mInvInertia;	/// The inverse inertia of the object.

	CollisionShape*	mColShape;	/// The collision shape of the object
};

