#pragma once
#define GLM_FORCE_CXX14
#include <glm/vec3.hpp>

class Object
{
public:
    Object(double mass = 0.0);
    virtual ~Object() = 0;

    virtual void print() = 0;

    // Accessors/Mutators
    void setMass(double mass);

private:
    double      mMass;
    glm::vec3   mPos;
    glm::vec3   mRot;
    glm::vec3   mForce;
    glm::vec3   mTorque;
};

