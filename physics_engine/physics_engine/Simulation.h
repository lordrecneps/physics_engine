#pragma once
#include <vector>

#include "Object.h"

class Simulation
{
public:
	Simulation();
	~Simulation();
    
    bool initialize();
    void step();

private:
    std::vector<Object*>    mObjList;
};
