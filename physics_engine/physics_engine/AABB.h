#pragma once
#include "Object.h"
#include <glm/vec3.hpp>

class AABB :
    public Object
{
public:
    AABB(double wx, double wy, double wz);
    ~AABB();

    void print();

    glm::vec3 getDim();

    void setDim(glm::vec3& dim);

private:
    glm::vec3   mDim;   ///< Dimensions of the box along the three axes. Full length, not half length.
};

