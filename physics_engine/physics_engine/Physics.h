#pragma once
#include <vector>

#include "Object.h"

class Physics
{
public:
	Physics(std::vector<Object*>& objList);
	~Physics();

    void initialise();

private:
    std::vector<Object*>&   mObjList;
};
