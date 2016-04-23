#pragma once
#include "ObjectTypes.h"

class CollisionShape
{
public:
	CollisionShape(ObjectType type);
	virtual ~CollisionShape() = 0;

    ObjectType type() const;

private:
    ObjectType  mType;  ///< The type of object representing this collision shape.
};