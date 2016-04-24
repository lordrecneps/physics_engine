#pragma once
#include <glm/vec2.hpp>
#include "CollisionShape.h"

class PlaneCollisionShape : public CollisionShape
{
public:
    /*!
     * @param w Width
     * @param h Height
     */
    PlaneCollisionShape(double w, double h);
    ~PlaneCollisionShape();

    glm::dvec2 getDim() const;

private:
    glm::dvec2 mDim;
};

