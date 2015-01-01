#include "System.h"
#include <cassert>

System::System(World* world) : world(world)
{

}

System::~System()
{

}

void System::update(sf::Time deltaTime)
{

}

void System::render(float alpha)
{

}

void System::advance()
{
    for (const auto& e : toAdd)
    {
        // If we've reached this point, we should be able to assume that the Entity isn't
        // in the System since we made a check in markToAdd
        assert(!hasEntity(*e));
        internalAddEntity(*e);
    }

    for (const auto& e : toRemove)
    {
        // Similar to comment above about adding
        assert(hasEntity(*e));
        internalRemoveEntity(*e);
    }
}

void System::inspectEntity(Entity& e)
{
    LOG(DEBUG) << "Inspecting Entity #" << e.getID();

    if (checkRequirements(e))
    {
        markToAdd(e);
    }
}

void System::markToRemove(Entity& e)
{
    if (!hasEntity(e))
    {
        LOG(WARNING) << "Tried to remove an Entity from a System which doesn't contain it";
        return;
    }

    if (toRemove.find(&e) != toRemove.end())
    {
        LOG(WARNING) << "Marked the same Entity for removal more than once";
        return;
    }

    toRemove.insert(&e);

    LOG(DEBUG) << "Marked Entity #" << e.getID() << " to be removed";
}

void System::markToAdd(Entity& e)
{
    if (hasEntity(e))
    {
        LOG(WARNING) << "Tried to add an Entity to a System which already contains it";
        return;
    }

    if (toAdd.find(&e) != toAdd.end())
    {
        LOG(WARNING) << "Marked the same Entity for adding more than once";
        return;
    }

    toAdd.insert(&e);

    LOG(DEBUG) << "Marked Entity #" << e.getID() << " to be added";
}

World* System::getWorld() const
{
    return world;
}