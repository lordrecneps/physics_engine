#pragma once
#include "Object.h"
class Sphere :
    public Object
{
public:
    Sphere(double radius);
    ~Sphere();

    void print();

    double getRadius();

    void setRadius(double radius);

private:
    double      mRadius;
};

