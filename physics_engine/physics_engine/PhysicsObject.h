#pragma once

#define GLM_FORCE_CXX14
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

private:
	double      mMass;
	glm::vec3   mPos;
	glm::vec3   mRot;
	glm::vec3   mForce;
	glm::vec3   mTorque;
};

