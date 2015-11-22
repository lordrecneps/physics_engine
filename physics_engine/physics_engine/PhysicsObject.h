#pragma once

#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

class PhysicsObject
{
public:
	PhysicsObject();
	~PhysicsObject();

	// Accessors/Mutators
	void setMass(double mass);

	void setPos(glm::vec3& pos);

	void setRot(glm::quat& rot);

	void setPose(glm::vec3& pos, glm::quat& rot);

    double mass();

    glm::vec3 pos();

    glm::quat rot();

    glm::vec3 vel();

    glm::vec3 angVel();

    glm::vec3 force();

    glm::vec3 torque();

private:
	double      mMass;
	glm::vec3   mPos;
	glm::quat   mRot;
    glm::vec3   mVel;
    glm::vec3   mAngVel;
	glm::vec3   mForce;
	glm::vec3   mTorque;

};

