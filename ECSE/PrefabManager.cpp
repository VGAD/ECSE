#include "PrefabManager.h"
#include "World.h"
#include "PrefabComponent.h"

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

    if (storeNames)
    {
        world.attachComponent<PrefabComponent>(entId)->prefabName = name;
    }

    applyPrefab(name, world, world.getEntity(entId));
    world.registerEntity(entId);

    return entId;
}

void PrefabManager::applyPrefab(std::string name, World& world, ECSE::Entity* entity) const
{
    auto& prefab = getPrefab(name);

    prefab(world, entity);
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