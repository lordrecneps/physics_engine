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
    reader.read("test_scene.xml", mObjList);

    std::for_each(mObjList.begin(), mObjList.end(), [](Object* o) {
        o->print();
    });

    Physics dynamics(mObjList);
    Renderer graphics(mObjList);
}

void Simulation::step()
{
}
