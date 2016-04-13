#include "SphereCollisionShape.h"

SphereCollisionShape::SphereCollisionShape()
{
}

SphereCollisionShape::SphereCollisionShape(double radius)
	: mRadius(radius)
{
}

SphereCollisionShape::~SphereCollisionShape()
{
}

double SphereCollisionShape::radius() const
{
	return mRadius;
}
