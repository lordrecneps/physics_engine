#include <iostream>
#include "AABB.h"



AABB::AABB(double wx, double wy, double wz)
    : mDim(wx, wy, wz)
{
}



AABB::~AABB()
{
}

void AABB::print()
{
    std::cout << "AABB; dim = [" << mDim.x << ", " << mDim.y << ", " << mDim.z << "]" << std::endl;
}
