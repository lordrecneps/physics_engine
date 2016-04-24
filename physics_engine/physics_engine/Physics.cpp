#include <iostream>
#include <glm/gtx/string_cast.hpp>
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
        auto po1_ptr = &colPair.first->phys();
        auto po2_ptr = &colPair.second->phys();
        if(Collision::areObjectsColliding(*po1_ptr, *po2_ptr, &colData))
        {
            if(po1_ptr->collisionShape()->type() > po2_ptr->collisionShape()->type())
                std::swap(po1_ptr, po2_ptr);
            
            auto& po1 = *po1_ptr;
            auto& po2 = *po2_ptr;

            if(glm::dot(po1.vel(), colData.normal) >= 0 && glm::dot(po2.vel(), -colData.normal) >= 0)
                continue;

            auto CoR = 0.7;
            auto v_ab = po1.vel() - po2.vel();
            auto J = -(1 + CoR) * (glm::dot(v_ab, colData.normal)) / (po1.invMass() + po2.invMass());

            std::cout << "J: " << J << ", v_ab: " << glm::to_string(v_ab) << ", n: " << glm::to_string(colData.normal) << std::endl;

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