#include <algorithm>
#include <functional>

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
}

void Simulation::step()
{
}
