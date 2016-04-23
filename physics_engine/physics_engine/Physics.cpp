#include <iostream>

#include "Collision.h"
#include "Physics.h"

typedef std::pair<Object*, Object*> CollisionPair;

Physics::Physics(std::vector<Object*>& objList)
    : mObjList(objList)
{
}

Physics::~Physics()
{
}

void Physics::initialise()
{
}

bool Physics::update()
{
    std::vector<CollisionPair> potentialCollisions;
	// broadphase
    for(auto i = mObjList.begin(); i != mObjList.end(); ++i)
    {
        for(auto j = i + 1; j != mObjList.end(); ++j)
        {
            potentialCollisions.push_back(std::make_pair(*i, *j));
        }
    }

	// narrowphase
    for(const auto& colPair : potentialCollisions)
    {
        CollisionData colData;
        PhysicsObject& po1 = colPair.first->phys();
        PhysicsObject& po2 = colPair.second->phys();
        if(Collision::areObjectsColliding(po1, po2, &colData))
        {
            //colPair.first->phys().setVel(glm::dvec3(0.0));
            //colPair.second->phys().setVel(glm::dvec3(0.0));
            auto CoR = 0.7;
            auto v_ab = po1.vel() - po2.vel();
            auto J = -(1 + CoR) * (glm::dot(v_ab, colData.normal)) / (po1.invMass() + po2.invMass());

            po1.setVel(po1.vel() + J * po1.invMass() * colData.normal);
            po2.setVel(po2.vel() - J * po2.invMass() * colData.normal);
        }
    }

	for(const auto& obj : mObjList)
	{
		obj->phys().update();
	}

    return true;
}