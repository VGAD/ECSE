#include "TransformSystem.h"

void TransformSystem::advance()
{
    SetSystem::advance();

    for (Entity* entity : getEntities())
    {
        TransformComponent* tc = entity->getComponent<TransformComponent>();
        if (!tc->enabled) continue;

        tc->advance();
    }
}

bool TransformSystem::checkRequirements(const Entity& e) const
{
    if (!e.getComponent<TransformComponent>()) return false;

    return true;
}