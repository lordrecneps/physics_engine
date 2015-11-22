#include <algorithm>
#include <functional>

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

void Simulation::initialize()
{
    SceneReader reader;
    Camera cam;
    reader.read("test_scene.xml");
    reader.readObjects(mObjList);
    reader.readCamera(cam);

    std::for_each(mObjList.begin(), mObjList.end(), [](Object* o) {
        o->print();
    });

    Physics dynamics(mObjList);
    Renderer graphics(mObjList);

    graphics.setCamera(cam);

    graphics.init();
    graphics.render();
}

void Simulation::step()
{
}
