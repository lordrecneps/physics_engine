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

    std::cout << "papapapapappapapaa" << std::endl;
    return 0;
}