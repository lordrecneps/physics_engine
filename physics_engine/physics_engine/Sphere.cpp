#include <iostream>

#include "Sphere.h"



Sphere::Sphere(double radius)
    : Object(ObjectType::eSPHERE)
    , mRadius(radius)
{
}


Sphere::~Sphere()
{
}

void Sphere::print()
{
    std::cout << "sphere; radius = " << mRadius << std::endl;
}

double Sphere::getRadius()
{
    return mRadius;
}

void Sphere::setRadius(double radius)
{
    mRadius = radius;
}
