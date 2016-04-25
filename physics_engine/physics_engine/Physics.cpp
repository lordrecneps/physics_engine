#include <algorithm>
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
    constexpr auto time_step = 0.1;
    constexpr auto inv_time_step = 1.0 / time_step;

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

            auto CoR = 0.7; // coefficient of restitution.
            auto v_ab = po1.vel() - po2.vel();
            auto sum_inv_masses = po1.invMass() + po2.invMass();
            auto J = -(1 + CoR) * (glm::dot(v_ab, colData.normal)) / (sum_inv_masses);

            std::cout << "J: " << J << ", v_ab: " << glm::to_string(v_ab) << ", n: " << glm::to_string(colData.normal) << std::endl;
            
            auto response1 = std::max(J*po1.invMass(), colData.penetration * inv_time_step * po1.invMass() / sum_inv_masses);
            auto response2 = std::max(J*po2.invMass(), colData.penetration * inv_time_step * po2.invMass() / sum_inv_masses);
            po1.setVel(po1.vel() + response1 * colData.normal);
            po2.setVel(po2.vel() - response2 * colData.normal);
        }
    }

	for(const auto& obj : mObjList)
	{
		obj->phys().update(time_step);
	}

    return true;
}