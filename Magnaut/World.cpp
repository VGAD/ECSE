#include "World.h"

World::World()
{
}

World::~World()
{
}

void World::update(sf::Time deltaTime)
{
    for (auto& pair : systems)
    {
        pair.second->update(deltaTime);
    }
}

void World::advance()
{
    for (auto& pair : systems)
    {
        pair.second->advance();
    }
}

void World::render(float alpha)
{
    for (auto& pair : systems)
    {
        pair.second->render(alpha);
    }
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