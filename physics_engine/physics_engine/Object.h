#pragma once

#include "ObjectTypes.h"
#include "PhysicsObject.h"
#include "RenderObject.h"

/*!
 *  @brief  Base class for all objects/shapes.
 */
class Object
{
public:
    Object(ObjectType type, double mass = 0.0);
    virtual ~Object() = 0;

    /*!
     *  @brief  Prints diagnostic message pertaining to the object.
     */
    virtual void print() = 0;

    /*!
     *  @brief Returns the physical properties of the object.
     */
	PhysicsObject& phys();

    /*!
    *  @brief Returns the render properties of the object.
    */
    RenderObject& rend();

    /*!
     *  @brief  Returns the enumerated type of the object.
     */
    ObjectType type();

protected:
	PhysicsObject	mPhysicsObj;	/// Physics properties.
	RenderObject	mRenderObj;	    /// Render properties.
    ObjectType      mType;          /// The type of the object.
};

