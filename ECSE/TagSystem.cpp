#include "TagSystem.h"
#include "TagComponent.h"

namespace ECSE {

std::set<Entity*> TagSystem::findWithTag(size_t tag) const
{
    std::set<Entity*> results;

    for (auto entity : getEntities())
    {
        if (entity->getComponent<TagComponent>()->hasTag(tag))
        {
            results.insert(entity);
        }
    }

    return results;
}

bool TagSystem::checkRequirements(const Entity& e) const
{
    if (!e.getComponent<TagComponent>()) return false;

    return true;
}

}
