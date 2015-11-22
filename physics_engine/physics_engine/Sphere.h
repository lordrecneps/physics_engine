#pragma once
#include "Object.h"

/*!
*  @brief  A sphere. Yep.
*/
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
    double      mRadius;    /// The radius of the sphere.
};

