#pragma once
#include "Object.h"
#include <glm/vec3.hpp>

/*!
 *  @brief  An axis-aligned bounding box.
 */
class AABB :
    public Object
{
public:
    AABB(double wx, double wy, double wz);
    ~AABB();

    void print();

    /*!
     *  @brief  Returns the dimensions of the box in terms of half-lengths.
     *  @return The dimensions of the box along the three axes in half-lengths. I.e. distance from the centre to the end.
     */
    glm::dvec3 getDim();

    /*!
     *  @brief  Sets the dimensions of the box.
     *  @param[in]  dim The dimensions to set the box to.
     */
    void setDim(glm::dvec3& dim);

private:
    glm::dvec3   mDim;   /// Dimensions of the box along the three axes. Half lengths.
};

