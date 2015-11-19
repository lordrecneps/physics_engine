#pragma once
#include "Object.h"
class Sphere :
    public Object
{
public:
    Sphere(double radius);
    ~Sphere();

private:
    double      mRadius;
};

