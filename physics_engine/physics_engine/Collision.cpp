#include "Collision.h"

#include "SphereCollisionShape.h"

Collision::Collision()
{
}

Collision::~Collision()
{
}

bool Collision::sphereSphereCollision(const PhysicsObject& obj0, const PhysicsObject& obj1, CollisionData* colDataOut)
{
	const SphereCollisionShape* sphere0 = dynamic_cast<const SphereCollisionShape*>(obj0.collisionShape());
	const SphereCollisionShape* sphere1 = dynamic_cast<const SphereCollisionShape*>(obj1.collisionShape());

	if(!sphere0 || !sphere1)
	{
		return false;
	}

	float radiusSum = sphere0->radius() + sphere1->radius();
	glm::vec3 ab = obj0.pos() - obj1.pos();
	float distSquared = glm::dot(ab, ab);

	if(distSquared <= radiusSum*radiusSum)
	{
		if(colDataOut)
		{
			colDataOut->penetration = radiusSum - glm::sqrt(distSquared);
			colDataOut->normal = glm::normalize(ab);
			colDataOut->point = obj0.pos() - colDataOut->normal*(sphere0->radius() - colDataOut->penetration);
		}
		return true;
	}
	return false;
}