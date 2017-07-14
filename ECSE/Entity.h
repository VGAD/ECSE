#pragma once

#include "Component.h"
#include <boost/unordered_map.hpp>
#include <cstdint>

namespace ECSE
{

//! A container for Components.
/*!
* Components can only be attached through the public World function attachComponent. This
* is because Components should only be attached before the Entity is registered with any
* systems. This also helps to avoid attaching Components from a different World.
*/
class Entity
{
    friend class EntityManager;
    friend class World;

public:
    //! The type used for Entity IDs.
    typedef std::uint64_t ID;

    //! Construct the Entity.
    Entity();

    //! Destroy the Entity.
    ~Entity();

    //! Get the Entity's ID.
    inline ID getID() const
    {
        return id;
    }

    //! Get a pointer to the Component of a certain type.
    /*!
    * \return A pointer to the component of that type, or nullptr if it doesn't have one.
    */
    template <typename ComponentType>
    ComponentType* getComponent() const;

    //! Get the Entity's Components.
    /*!
    * \return A reference to the Entity's components, mapped by type hash code.
    */
    const boost::unordered_map<size_t, Component*>& getComponents() const;


    // Data

    //! The invalid ID.
    const static ID invalidID;

private:
    //! Attach a component.
    /*!
    * \param component A pointer to the Component to attach.
    */
    template <typename ComponentType>
    void attachComponent(ComponentType* component);

    // Use a boost unordered map because MSVC's STL unordered map is slower than molasses on a cold winter's day.
    boost::unordered_map<size_t, Component*> components;    //!< Map from type hash code to a pointer to the Component.
    ID id;                                                  //!< Unique identifier for this Entity.
    bool registered = false;                                //!< Whether this has been registered in any Systems yet.
};

/////////////////
// Implementation

template <typename ComponentType>
ComponentType* Entity::getComponent() const
{
    static_assert(std::is_base_of<Component, ComponentType>::value,
                  "ComponentType must be a descendant of Component!");

    size_t hashCode = typeid(ComponentType).hash_code();
    auto it = components.find(hashCode);

    if (it == components.end())
    {
        return nullptr;
    }

    return static_cast<ComponentType*>(it->second);
}

template <typename ComponentType>
void Entity::attachComponent(ComponentType* component)
{
    static_assert(std::is_base_of<Component, ComponentType>::value,
                  "ComponentType must be a descendant of Component!");

    size_t hashCode = typeid(ComponentType).hash_code();
    auto it = components.find(hashCode);

    if (it != components.end())
    {
        std::stringstream ss;
        ss << "A Component of type \"" << typeid(ComponentType).name() << "\" is already attached to this Entity!";

        throw std::runtime_error(ss.str());
    }

    components[hashCode] = component;

    // Tell the Component it's been attached
    component->attached(this);
}

}
