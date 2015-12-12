#include "SphereCollisionShape.h"

SphereCollisionShape::SphereCollisionShape()
{
}

SphereCollisionShape::SphereCollisionShape(float radius)
	: mRadius(radius)
{
}

SphereCollisionShape::~SphereCollisionShape()
{
}

SphereCollisionShape::radius() const
{
	return mRadius;
}
