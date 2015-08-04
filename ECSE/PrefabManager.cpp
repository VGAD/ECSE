#include "PrefabManager.h"
#include "World.h"

namespace ECSE {

void PrefabManager::addPrefab(std::string name, const Prefab& prefab)
{
    if (prefabs.find(name) != prefabs.end())
    {
        throw std::runtime_error("Prefab with name \"" + name + "\" already exists");
    }

    prefabs[name] = prefab;
}

Entity::ID PrefabManager::createEntity(std::string name, World& world) const
{
    auto entId = world.createEntity();
    applyPrefab(name, world, entId);
    world.registerEntity(entId);

    return entId;
}

void PrefabManager::applyPrefab(std::string name, World& world, Entity::ID entId) const
{
    auto& prefab = getPrefab(name);

    prefab(world, entId);
}

const PrefabManager::Prefab& PrefabManager::getPrefab(std::string name) const
{
    auto it = prefabs.find(name);

    if (it == prefabs.end())
    {
        throw std::runtime_error("Prefab with name \"" + name + "\" does not exist");
    }

    return it->second;
}

}