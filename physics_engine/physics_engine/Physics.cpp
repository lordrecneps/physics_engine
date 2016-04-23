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
    for(auto colPair : potentialCollisions)
    {
        CollisionData colData;
        if(Collision::areObjectsColliding(colPair.first->phys(), colPair.second->phys(), &colData))
        {
            colPair.first->phys().setVel(glm::dvec3(0.0));
            colPair.second->phys().setVel(glm::dvec3(0.0));
        }
    }

	for(const auto& obj : mObjList)
	{
		obj->phys().update();
	}

    return true;
}