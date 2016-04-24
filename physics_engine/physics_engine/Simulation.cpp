#include <algorithm>
#include <chrono>
#include <functional>
#include <iostream>
#include <thread>

#include "GLFWWindow.h"
#include "Physics.h"
#include "Renderer.h"
#include "SceneReader.h"
#include "Simulation.h"



Simulation::Simulation()
    :mState(State::RUN)
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
    
    mWindow.reset(new GLFWWindow());

    reader.readObjects(mObjList);
    reader.readCamera(mWindow->getCamera());
    reader.readLight(light);

    std::for_each(mObjList.begin(), mObjList.end(), [](Object* o) {
        o->print();
    });

    mWindow->init(1024, 768);
    mPhysics.reset(new Physics(mObjList));
	mRenderer.reset(new Renderer(mObjList));

    mRenderer->setCamera(mWindow->getCamera());
    mRenderer->setLight(light);

    if(!mRenderer->init())
    {
        std::cerr << "Failed to initialize renderer." << std::endl;
        return false;
    }
    
    return true;
}

bool Simulation::step()
{
    State nextState = mWindow->update();

    switch(nextState)
    {
        case State::ERROR:
        {
            std::cout << "Error encountered in window class." << std::endl;
            return false;
        }
        case State::EXIT:
        {
            std::cout << "Exit triggered by window." << std::endl;
            return false;
        }
        case State::STEP:
        {
            if(mState == State::PAUSE)
                mState = State::STEP;
            break;
        }
        case State::PAUSE:
        {
            mState = State::PAUSE;
            break;
        }
        case State::UNPAUSE:
        {
            if(mState == State::PAUSE)
                mState = State::RUN;
            break;
        }
        case State::RUN:
        {
            break;
        }
        case State::INVALID:
        {
            std::cout << "Shouldn't be here." << std::endl;
            break;
        }
    }

    if(mState == State::RUN || mState == State::STEP)
    {
        if(!mPhysics->update())
        {
            std::cout << "Exit triggered by physics." << std::endl;
            return false;
        }
    }

    if(!mRenderer->render())
    {
        std::cout << "Exit triggered by renderer." << std::endl;
        return false;
    }

    if(mState == State::STEP)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        mState = State::PAUSE;
    }

    return true;
}

void Simulation::run()
{
    // TODO: to be frame rate independent, need to run physics multiple times vs once for renderer
	// while(windows.notClosed)
	// { physics, renderer update }
	while(step())
    {
        ;
    }
}



