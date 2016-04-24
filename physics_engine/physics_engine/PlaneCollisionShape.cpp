#include "PlaneCollisionShape.h"



PlaneCollisionShape::PlaneCollisionShape(double w, double h)
    : CollisionShape(eQUAD)
    , mDim(w, h)
{
}


PlaneCollisionShape::~PlaneCollisionShape()
{
}

glm::dvec2 PlaneCollisionShape::getDim() const
{
    return mDim;
}
