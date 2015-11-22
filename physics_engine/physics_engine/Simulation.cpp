#include <algorithm>
#include <functional>
#include <iostream>

#include "Physics.h"
#include "Renderer.h"
#include "SceneReader.h"
#include "Simulation.h"



Simulation::Simulation()
{
}


Simulation::~Simulation()
{
}

bool Simulation::initialize()
{
    SceneReader reader;
    Camera cam;
    PointLight light;

    if(!reader.read("test_scene.xml"))
    {
        std::cerr << "Failed to read scene file. Aborting." << std::endl;
        return false;
    }

    reader.readObjects(mObjList);
    reader.readCamera(cam);
    reader.readLight(light);

    std::for_each(mObjList.begin(), mObjList.end(), [](Object* o) {
        o->print();
    });

    Physics dynamics(mObjList);
    Renderer graphics(mObjList);

    graphics.setCamera(cam);
    graphics.setLight(light);

    if(!graphics.init())
    {
        std::cerr << "Failed to initialize renderer." << std::endl;
        return false;
    }
    graphics.render();

    return true;
}

void Simulation::step()
{
}
