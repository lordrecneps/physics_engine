#pragma once

#include "CollisionData.h"
#include "PhysicsObject.h"
#include "SphereCollisionShape.h"

namespace Collision
{
	/*!
	 *  @brief  Check for sphere sphere collision
	 *	@param[in]	obj0		First of the pair of physics object to be checked
	 *	@param[in]	obj1		Second of the pair of physics object to be checked
	 *	@param[in]	colDataOut	The data from the collision, if any
	 *	@return		True if colliding
	 */
    bool areObjectsColliding(const PhysicsObject& obj0, const PhysicsObject& obj1, CollisionData* colDataOut = nullptr);

    bool sphereSphereCollision(const PhysicsObject& obj0, const PhysicsObject& obj1, CollisionData* colDataOut = nullptr);
};

