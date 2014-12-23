#include "World.h"

World::World()
{
}

World::~World()
{
}

void World::update(double deltaTime)
{

}

void World::destroyEntity(Entity::ID id)
{
    Entity* e = getEntity(id);
    for (const auto& pair : e->getComponents())
    {
        destroyComponent(pair.second);
    }

    EntityManager::destroyEntity(id);
}