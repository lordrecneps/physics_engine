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

float SphereCollisionShape::radius() const
{
	return mRadius;
}
