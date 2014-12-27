#include "SetSystem.h"

SetSystem::~SetSystem()
{

}

bool SetSystem::hasEntity(const Entity& e) const
{
    return entities.find(const_cast<Entity*>(&e)) != entities.end();
}

void SetSystem::internalAddEntity(Entity& e)
{
    entities.insert(&e);
}

void SetSystem::internalRemoveEntity(Entity& e)
{
    entities.erase(&e);
}