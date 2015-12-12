#pragma once

#include "CollisionShape.h"

class SphereCollisionShape : public CollisionShape
{
public:
	SphereCollisionShape();
	SphereCollisionShape(float radius);
	~SphereCollisionShape();

	float radius() const;

private:
	float mRadius;	/// The radius of the collision sphere
};

