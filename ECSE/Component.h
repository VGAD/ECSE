#pragma once

namespace ECSE
{

//! A class which holds data about an Entity.
/*!
* Components may include convenience functions for operating on their data, but should
* generally not be aware of other Components. Any function that needs to deal with more
* than one Component should probably go in a System.
*/
class Component
{
public:
    friend class Entity;
    virtual ~Component() {}

    //! Redefine this in subclasses to indicate that they are a polymorphic extension of a given Component type.
    /*!
    * This allows for component polymorphism. This component will be returned when entity->getComponent() is
    * called for both its own type and ExtendsComponent.
    *
    * Usage example: you may have a Damage component with a function getDamage() that returns the damage dealt.
    * You could use this function add a SpeedDamage component that descends from Damage and that returns a
    * higher value from getDamage() when its speed is higher. When you call entity->getComponent<Damage>() later,
    * a pointer to the SpeedDamage component will be returned.
    *
    * If the type is Component, it doesn't extend anything. Note that it is possible to have multiple levels of
    * extension (e.g. ComponentC extends ComponentB extends ComponentA), though it's recommended to maintain
    * a fairly flat Component hierarchy.
    */
    using ExtendsComponent = Component;

    //! Whether Systems should update this.
    /*!
    * Effectively, the System should behave as if this was not attached to its Entity.
    */
    bool enabled = true;

protected:
    //! Called when this is attached to an Entity.
    /*!
    * This is a good place to check requirements and/or notify other Components.
    * \param e The Entity to which this was attached.
    */
    virtual void attached(Entity* e) {};
};

}
