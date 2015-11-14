#include "State.h"

namespace ECSE
{

void State::update(sf::Time deltaTime)
{
    for (auto it = sideRoutines.begin(); it != sideRoutines.end();)
    {
        // Execute coroutine
        bool stopRoutine = (*it)();

        if (stopRoutine)
        {
            sideRoutines.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void State::startSideRoutine(SideRoutine fn)
{
    sideRoutines.push_back(fn);
}

}