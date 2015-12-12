#pragma once
#include <memory>
#include <vector>

#include "Physics.h"
#include "Object.h"
#include "Renderer.h"
#include "Window.h"


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
    bool step();

    /*!
     *  @brief  Main update loop. Updates the renderer/physics engine until an exit is triggered by the user.
     */
    void run();

private:
    std::vector<Object*>		mObjList;   /// A list of all the objects in the scene.
	std::unique_ptr<Renderer>	mRenderer;  /// The renderer for the simulation. This can be a raster, ray tracing, etc. engine.
	std::unique_ptr<Physics>	mPhysics;   /// The dynamics class that will be used to simulate the physics.
    std::unique_ptr<Window>	    mWindow;    /// The window class to display the results of the renderer.
};
