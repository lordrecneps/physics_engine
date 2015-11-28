#pragma once
#include <vector>

#include "Object.h"

/*!
 *  @brief  This class handles the dynamics of all the objects in the scene.
 */
class Physics
{
public:
	Physics(std::vector<Object*>& objList);
	~Physics();

    /*!
     *  @brief  Initializes the physics engine.
     */
    void initialise();

	/*!
	 *	@brief	Update physics
	 */
	bool update();

private:
    std::vector<Object*>&   mObjList;   /// Reference to the list of all objects in the scene.
};
