#pragma once
#include "Object.h"

/*
 *  @brief  A finite rectangular plane. By default it is centered at the origin and lies in the x-z plane;
            i.e. the normal of the plane is (0, 0, 1).
 */
class Quad :
    public Object
{
public:
    Quad(double wx, double wz);
    ~Quad();

    void print();

    /*!
    *  @brief  Returns the dimensions of the quad in terms of half-lengths.
    *  @return The dimensions of the quad along the x and z axes in half-lengths. I.e. distance from the centre to the end.
    */
    glm::dvec2 getDim();

    /*!
    *  @brief  Sets the dimensions of the quad.
    *  @param[in]  dim The dimensions to set the quad to.
    */
    void setDim(glm::dvec2& dim);

private:
    glm::dvec2   mDim;   /// Dimension of the quad along the x and z axes.
};

