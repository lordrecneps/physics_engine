#include "CollisionShape.h"


CollisionShape::CollisionShape(ObjectType type)
    : mType(type)
{
}

CollisionShape::~CollisionShape()
{
}

ObjectType CollisionShape::type() const
{
    return mType;
}
