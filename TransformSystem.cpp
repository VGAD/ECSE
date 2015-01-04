#include "TransformSystem.h"

void TransformSystem::advance()
{
    SetSystem::advance();

    for (Entity* entity : getEntities())
    {
        entity->getComponent<TransformComponent>()->advance();
    }
}

bool TransformSystem::checkRequirements(const Entity& e) const
{
    if (!e.getComponent<TransformComponent>()) return false;

    return true;
}