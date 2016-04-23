#include "SphereCollisionShape.h"

SphereCollisionShape::SphereCollisionShape()
    : CollisionShape(eSPHERE)
    , mRadius(1.0)
{
}

SphereCollisionShape::SphereCollisionShape(double radius)
	: CollisionShape(eSPHERE)
    , mRadius(radius)
{
}

SphereCollisionShape::~SphereCollisionShape()
{
}

double SphereCollisionShape::radius() const
{
	return mRadius;
}
