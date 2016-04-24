#pragma once

#include "Camera.h"
#include "Simulation.h"

class Window
{
public:
    Window();
    virtual ~Window() = 0;

    virtual bool init(uint32_t width, uint32_t height) = 0;

    virtual Simulation::State update() = 0;

    virtual Camera& getCamera() = 0;
};

