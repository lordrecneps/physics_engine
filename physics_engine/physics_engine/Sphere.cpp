#include <iostream>

#include "Sphere.h"



Sphere::Sphere(double radius)
    : mRadius(radius)
{
}


Sphere::~Sphere()
{
}

void Sphere::print()
{
    std::cout << "sphere; radius = " << mRadius << std::endl;
}
