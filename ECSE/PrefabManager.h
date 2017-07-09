#pragma once

#include <functional>
#include <map>
#include "Entity.h"
#include "World.h"

namespace ECSE
{

//! Holds prefabs, which allow entities to be constructed from predefined components and settings.
class PrefabManager
{
public:
    using Properties = std::map<std::string, std::string>;

    //! A function which applies prefab settings to an entity.
    /*!
    * This function receives a reference to a pre-created Entity which exists in the
    * given World and should add components and configure them. The function should
    * not register the Entity, as other prefabs may also be applied afterward.
    * Optionally, the prefab may make use of values in the props map.
    */
    using Prefab = std::function<void(World& world, ECSE::Entity& entity, Properties props)>;

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
    * \param props A map from optional property names to string values.
    * \return The new entity's ID.
    */
    Entity::ID createEntity(std::string name, World& world, Properties props = Properties()) const;

    //! Apply a prefab's settings to an entity.
    /*!
    * \param name The name of the prefab.
    * \param world The world in which the entity exists.
    * \param entity The entity.
    * \param props A map from optional property names to string values.
    */
    void applyPrefab(std::string name, World& world, ECSE::Entity& entity, Properties props = Properties()) const;

    //! Check if a prefab exists.
    /*!
    * \param name The prefab's name
    * \return Whether or not the prefab exists.
    */
    bool hasPrefab(std::string name) const;

    //! Set whether or not to store prefab names in entities created using createEntity().
    /*!
    * \param storeNames If true, a PrefabComponent with the prefab name will be added automatically
    *                   to any entities created by this manager.
    */
    inline void setStoreNames(bool storeNames) { this->storeNames = storeNames; }

private:
    std::map<std::string, Prefab> prefabs;  //! Map from prefab name to prefab function.

    //! Get a prefab by its name, or throw an exception if it doesn't exist.
    /*!
    * \param name The prefab's name.
    * \return The prefab.
    */
    const Prefab& getPrefab(std::string name) const;

    bool storeNames = true;     //! If true, a PrefabComponent will be added to created entities.
};

}
