#pragma once

#include <vector>
#include <map>
#include <SFML/Graphics.hpp>
#include "ComponentManager.h"
#include "EntityManager.h"
#include "System.h"

class Engine;
class WorldState;

//! Holds Systems and executes their functions.
/*!
* Also responsible for allocating and deallocating both Entities and Components.
*/
class World : public EntityManager, public ComponentManager
{
public:
    //! Create an empty World.
    World(WorldState* worldState);

    //! Destroy the World and its contents.
    virtual ~World();

    //! Perform the update step for all Systems.
    /*!
    * \param deltaTime The time elapsed in this update.
    */
    virtual void update(sf::Time deltaTime);

    //! Perform the advance step for all Systems.
    virtual void advance();

    //! Perform the render step for all Systems.
    /*!
    * \param alpha The amount of interpolation in this render.
    */
    virtual void render(float alpha);

    //! Destroy an Entity, removing it from the simulation.
    /*!
    * Automatically removes the Entity's Components.
    * 
    * \param id The ID of the Entity to be removed.
    */
    virtual void destroyEntity(Entity::ID id);

    //! Register an Entity with all the Systems.
    /*!
    * Must be called after the Entity has been created for it to actually do anything.
    *
    * \param id The ID of the Entity to be registered.
    */
    virtual void registerEntity(Entity::ID id);

    //! Attach a Component to an Entity.
    /*!
    * \param id The ID of the Entity.
    * \return A pointer to the Component.
    */
    template <typename ComponentType>
    ComponentType* attachComponent(Entity::ID id);

    //! Add a System of this type to the World.
    template <typename SystemType>
    void addSystem();

    //! Get the System of this type.
    /*!
    * \return A pointer to the System of the given type, or nullptr if it doesn't have one.
    */
    template <typename SystemType>
    SystemType* getSystem();

    //! Get the Engine to which this belongs.
    /*!
    * \return A pointer to the Engine to which this belongs.
    */
    inline Engine* getEngine() const
    {
        return engine;
    }

protected:
    Engine* engine = nullptr;           //!< The Engine to which this belongs.
    WorldState* worldState = nullptr;   //!< The WorldState to which this belongs.

private:
    std::map<size_t, std::unique_ptr<System>> systems;  //!< Map from System type hash code to the System itself.
};

/////////////////
// Implementation

template <typename ComponentType>
ComponentType* World::attachComponent(Entity::ID id)
{
    Entity* entity = getEntity(id);

    if (!entity)
    {
        std::stringstream ss;
        ss << "Tried to attach a Component to an Entity with an invalid id (#" << id << ")";

        throw std::runtime_error(ss.str());
    }

    ComponentType* component = createComponent<ComponentType>();
    entity->attachComponent(component);

    return component;
}

template <typename SystemType>
void World::addSystem()
{
    if (getSystem<SystemType>() != nullptr)
    {
        std::stringstream ss;
        ss << "Tried to add a \"" << typeid(SystemType).name() << "\" system more than once!";

        throw std::runtime_error(ss.str());
    }

    size_t hashCode = typeid(SystemType).hash_code();
    systems[hashCode] = std::unique_ptr<System>(new SystemType(this));
}

template <typename SystemType>
SystemType* World::getSystem()
{
    static_assert(std::is_base_of<System, SystemType>::value,
                  "SystemType must be a descendant of System!");

    size_t hashCode = typeid(SystemType).hash_code();
    auto it = systems.find(hashCode);

    if (it == systems.end())
    {
        return nullptr;
    }

    // We should be safe to do a static cast since we know exactly what type this is.
    return static_cast<SystemType*>(it->second.get());
}