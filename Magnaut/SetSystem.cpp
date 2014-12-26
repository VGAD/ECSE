#include "SetSystem.h"

SetSystem::~SetSystem()
{

}

bool SetSystem::hasEntity(const Entity& e) const
{
    return entities.find(&e) != entities.end();
}

void SetSystem::internalAddEntity(Entity& e)
{
    entities.insert(&e);
}

void SetSystem::internalAddEntity(Entity& e)
{
    entities.erase(&e);
}