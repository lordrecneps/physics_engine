#pragma once

#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

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
	void setMass(double mass);

	void setPos(glm::vec3& pos);

	void setRot(glm::quat& rot);

	void setPose(glm::vec3& pos, glm::quat& rot);

    double mass();

	double invMass();

    glm::vec3 pos();

    glm::quat rot();

    glm::vec3 vel();

    glm::vec3 angVel();

    glm::vec3 force();

    glm::vec3 torque();

private:
	double      mMass;          /// Mass of the object.
	double      mInvMass;       /// Inverse mass of the object.
	glm::vec3   mPos;           /// The position of the object.
	glm::quat   mRot;           /// The orientation of the object.
    glm::vec3   mVel;           /// The velocity of the object.
    glm::vec3   mAngVel;        /// The angular velocity of the object.
	glm::vec3   mForce;         /// The net force acting on the object.
	glm::vec3   mTorque;        /// The net torque acting on the object.

};

