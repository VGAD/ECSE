#pragma once

#include <vector>
#include <boost/unordered_map.hpp>
#include <SFML/Graphics.hpp>
#include <sstream>
#include "ComponentManager.h"
#include "EntityManager.h"
#include "System.h"

namespace ECSE
{

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
    explicit World(WorldState* worldState);

    //! Destroy the World and its contents.
    virtual ~World() override;

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
    * \param renderTarget The RenderTarget to draw to.
    */
    virtual void render(float alpha, sf::RenderTarget& renderTarget);

    //! Destroy an Entity, removing it from the simulation.
    /*!
    * Automatically removes the Entity's Components.
    * 
    * \param id The ID of the Entity to be removed.
    */
    virtual void destroyEntity(Entity::ID id) override;

    //! Register an Entity with all the Systems.
    /*!
    * Must be called after the Entity has been created for it to actually do anything.
    *
    * \param id The ID of the Entity to be registered.
    * \return The Entity that was registered.
    */
    virtual Entity* registerEntity(Entity::ID id);

    //! Attach a Component to an Entity.
    /*!
    * \tparam ComponentType The type of the component to add. Must be a descendant of Component.
    * \param id The ID of the Entity.
    * \return A pointer to the Component.
    */
    template <typename ComponentType>
    ComponentType* attachComponent(Entity::ID id);

    //! Attach a Component to an Entity, marking it also as its base type.
    /*!
    * This allows for component polymorphism. This component will be returned when entity->getComponent() is
    * called for both ComponentType and BaseType.
    * 
    * Usage example: you may have a Damage component with a function getDamage() that returns the damage dealt.
    * You could use this function add a SpeedDamage component that descends from Damage and that returns a
    * higher value from getDamage() when its speed is higher. When you call entity->getComponent<Damage>() later,
    * a pointer to the SpeedDamage component will be returned.
    * 
    * \tparam ComponentType The type of the component to add. Must be a descendant of Component.
    * \tparam BaseType The Component type from which ComponentType descends. Must be a descendant of Component.
    * \param id The ID of the Entity.
    * \return A pointer to the Component.
    */
    template <typename ComponentType, typename BaseType>
    ComponentType* attachComponent(Entity::ID id);

    //! Add a System of this type to the World.
    /*!
    * Note that the order in which Systems are added also determines the order in which they are updated.
    * \return The System that was added.
    */
    template <typename SystemType>
    SystemType* addSystem();

    //! Get the System of this type.
    /*!
    * \return A pointer to the System of the given type, or nullptr if it doesn't have one.
    */
    template <typename SystemType>
    SystemType* getSystem();

    //! Get the position of the SystemType in the ordered list.
    /*!
    * e.g. The first System to update will be at position 0, and the third will be at
    * position 2.
    *
    * This is useful to check when a System is added that it will update after a specific system.
    *
    * \return The System's update position, or -1 if it doesn't exist in the World.
    */
    template <typename SystemType>
    int getSystemPosition();

    //! Get the number of Systems in the World.
    /*!
    * \return The number of Systems.
    */
    int getSystemCount();

    //! Get the Engine to which this belongs.
    /*!
    * \return A pointer to the Engine to which this belongs.
    */
    Engine* getEngine() const;

protected:
    WorldState* worldState = nullptr;   //!< The WorldState to which this belongs.

private:
    // Use a boost unordered map because MSVC's STL unordered map is slower than molasses on a cold winter's day.
    boost::unordered_map<size_t, std::unique_ptr<System>> systems;  //!< Map from System type hash code to the System itself.
    std::vector<System*> orderedSystems;                            //!< Vector of Systems in preferred call order.
    bool systemsAdded = false;                                      //!< Whether Systems are finished being added.
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

    if (entity->registered)
    {
        std::stringstream ss;
        ss << "Tried to attach a Component to an Entity which has already been registered (#" << id << ")";

        throw std::runtime_error(ss.str());
    }

    ComponentType* component = createComponent<ComponentType>();
    entity->attachComponent(component);

    return component;
}

template <typename ComponentType, typename BaseType>
ComponentType* World::attachComponent(Entity::ID id)
{
    static_assert(std::is_base_of<BaseType, ComponentType>::value,
                  "ComponentType must be a descendant of BaseType!");

    auto* component = attachComponent<ComponentType>(id);

    Entity* entity = getEntity(id);
    entity->attachComponent(dynamic_cast<BaseType*>(component));

    return component;
}

template <typename SystemType>
SystemType* World::addSystem()
{
    static_assert(std::is_base_of<System, SystemType>::value,
                  "SystemType must be a descendant of System!");

    if (systemsAdded)
    {
        throw std::runtime_error("Tried to add a system after World has already started updating");
    }

    if (getSystem<SystemType>() != nullptr)
    {
        std::stringstream ss;
        ss << "Tried to add a \"" << typeid(SystemType).name() << "\" system more than once";

        throw std::runtime_error(ss.str());
    }

    std::unique_ptr<System> ptr = std::unique_ptr<System>(new SystemType(this));
    size_t hashCode = typeid(SystemType).hash_code();

    // Add pointer to map and the vector. Map owns the system while vector has just the pointer
    orderedSystems.push_back(ptr.get());
    systems[hashCode] = std::move(ptr);

    return static_cast<SystemType*>(systems[hashCode].get());
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

template <typename SystemType>
int World::getSystemPosition()
{
    System* system = getSystem<SystemType>();

    if (system == nullptr)
    {
        return -1;
    }

    auto iterator = find(orderedSystems.begin(), orderedSystems.end(), system);

    return iterator - orderedSystems.begin();
}

}
