#include <iostream>
#include "AABB.h"



AABB::AABB(double wx, double wy, double wz)
    : Object(ObjectType::eAABB)
    , mDim(wx, wy, wz)
{
}



AABB::~AABB()
{
}

void AABB::print()
{
    std::cout << "AABB; dim = [" << mDim.x << ", " << mDim.y << ", " << mDim.z << "]" << std::endl;
}

glm::dvec3 AABB::getDim()
{
    return mDim;
}

void AABB::setDim(glm::dvec3& dim)
{
    mDim = dim;
}
