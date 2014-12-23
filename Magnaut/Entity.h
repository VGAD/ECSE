#pragma once

#include "Component.h"
#include <map>
#include <cstdint>

//! A container for Components.
/*!
  Components should only be attached when the Entity is first created, and before it is passed
  off to a World's Systems to be inspected. This is why there is no detach function -- it is
  assumed that the map of Components will not change until the Entity is destroyed.
*/
class Entity
{
    friend class EntityManager;

public:
    //! The type used for Entity IDs.
    typedef std::uint64_t ID;

    //! Construct the Entity.
    Entity();

    //! Destroy the Entity.
    ~Entity();

    //! Get a pointer to the Component of a certain type.
    /*!
      \return A pointer to the component of that type.
    */
    template <typename ComponentType>
    ComponentType* getComponent() const;

    //! Get the Entity's Components.
    /*!
      \return A reference to the Entity's components, mapped by type hash code.
    */
    const std::map<size_t, Component*>& getComponents() const;

private:
    //! Attach a component.
    /*!
      \param component A pointer to the Component to attach.
    */
    template <typename ComponentType>
    void attachComponent(ComponentType* component);

    ID id;                                      //!< Unique identifier for this Entity.
    std::map<size_t, Component*> components;    //!< Map from type hash code to a pointer to the Component.
};

/////////////////
// Implementation

template <typename ComponentType>
ComponentType* Entity::getComponent() const
{
    static_assert(std::is_base_of<Component, ComponentType>::value,
                  "ComponentType must be a descendant of Component!");

    Component* component = components[typeid(ComponentType)];
    return static_cast<ComponentType*>(component);
}

template <typename ComponentType>
void Entity::attachComponent(ComponentType* component)
{
    static_assert(std::is_base_of<Component, ComponentType>::value,
                  "ComponentType must be a descendant of Component!");

    components[typeid(ComponentType)] = static_cast<ComponentType*>(component);
}