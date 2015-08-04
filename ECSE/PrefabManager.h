#pragma once

#include <functional>
#include <map>
#include "Entity.h"

namespace ECSE
{

//! Holds prefabs, which allow entities to be constructed from predefined components and settings.
class PrefabManager
{
public:
    //! A function which applies prefab settings to an entity.
    /*!
    * This function receives the ID of a pre-created Entity which exists in the
    * given World and should add components and configure them. The function should
    * not register the Entity, as other prefabs may also be applied afterward.
    */
    using Prefab = std::function<void(World& world, Entity::ID entId)>;

    //! Add a prefab to the manager.
    /*!
    * \param name The name of the prefab.
    * \param prefab The function which applies prefab settings.
    */
    void addPrefab(std::string name, const Prefab& prefab);

    //! Create an entity from a prefab and register it with the world.
    /*!
    * \param name The prefab's name.
    * \param world The world to which the entity should be added.
    * \return The new entity's ID.
    */
    Entity::ID createEntity(std::string name, World& world) const;

    //! Apply a prefab's settings to an entity.
    /*!
    * \param name The name of the prefab.
    * \param world The world in which the entity exists.
    * \param entId The entity's ID.
    */
    void applyPrefab(std::string name, World& world, Entity::ID entId) const;

private:
    std::map<std::string, Prefab> prefabs;  //! Map from prefab name to prefab function.

    //! Get a prefab by its name, or throw an exception if it doesn't exist.
    /*!
    * \param name The prefab's name.
    * \return The prefab.
    */
    const Prefab& getPrefab(std::string name) const;
};

}