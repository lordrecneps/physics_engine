#include <iostream>

#include "Quad.h"



Quad::Quad(double wx, double wz)
    : Object(ObjectType::eQUAD)
    , mDim(wx, wz)
{
    
}

Quad::~Quad()
{
}

void Quad::print()
{
    std::cout << "Quad; dim = [" << mDim.x << ", " << mDim.y << "]" << std::endl;
}

glm::dvec2 Quad::getDim()
{
    return mDim;
}

void Quad::setDim(glm::dvec2& dim)
{
    mDim = dim;
}
