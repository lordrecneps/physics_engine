#pragma once
#include <vector>

#include "Object.h"


/*!
 *  @brief  This class instantiates the physics engine and the renderer and reads/runs a particular scene.
 */
class Simulation
{
public:
	Simulation();
	~Simulation();
    
    /*!
     *  @brief  Read the scene file and instantiate the renderer and physics engine.
     */
    bool initialize();

    /*!
     *  @brief  Runs one frame of the simulation, i.e. steps forward in time by one time delta.
     */
    void step();

private:
    std::vector<Object*>    mObjList;   /// A list of all the objects in the scene.
};
