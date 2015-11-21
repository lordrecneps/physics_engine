#pragma once

#include <glm/vec3.hpp>


class PhysicsObject
{
public:
	PhysicsObject();
	~PhysicsObject();

	// Accessors/Mutators
	void setMass(double mass);

	void setPos(glm::vec3& pos);

	void setRot(glm::vec3& rot);

	void setPose(glm::vec3& pos, glm::vec3& rot);

    double mass();

    glm::vec3 pos();

    glm::vec3 rot();

    glm::vec3 vel();

    glm::vec3 angVel();

    glm::vec3 force();

    glm::vec3 torque();

private:
	double      mMass;
	glm::vec3   mPos;
	glm::vec3   mRot;
    glm::vec3   mVel;
    glm::vec3   mAngVel;
	glm::vec3   mForce;
	glm::vec3   mTorque;

};

