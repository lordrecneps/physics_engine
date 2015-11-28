#include <iostream>

#include "Simulation.h"

int main()
{
    Simulation sim;
    if(!sim.initialize())
    {
        std::cerr << "Failed to initialize simulation. Exiting." << std::endl;
        return 1;
    }

    sim.run();
    return 0;
}