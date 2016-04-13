#pragma once

#include "CollisionShape.h"

class SphereCollisionShape : public CollisionShape
{
public:
	SphereCollisionShape();
	SphereCollisionShape(double radius);
	~SphereCollisionShape();

    double radius() const;

private:
    double mRadius;	/// The radius of the collision sphere
};

