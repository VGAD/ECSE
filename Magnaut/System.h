#pragma once

#include <set>
#include <SFML/System.hpp>
#include "Entity.h"
#include "Common.h"

//! An interface which maintains a list of Entities and performs operations on their Components.
class System
{
public:
    //! Destroy the System.
    virtual ~System() {};

    //! Return whether an Entity is already in the System.
    /*!
      \param e The Entity to check.
      \return Whether the Entity is already in the System's internal systems.
    */
    virtual bool hasEntity(const Entity& e) const = 0;

    //! Called on an update step.
    /*!
      Each Entity should react to its current states, then prepare its next state.

      \param deltaTime The amount of elapsed time to simulate.
    */
    virtual void update(sf::Time deltaTime) = 0;

    //! Called on a render step.
    /*!
      Each Entity should be drawn, interpolated between its current and next state.

      \param alpha The amount of interpolation between the two states.
    */
    virtual void render(double alpha) = 0;

    //! Called on an advance step.
    /*!
      First, updates the internal list of Entities.
      Next, each Entity should update its current state to advance the simulation.

      If you override this, make sure to call System::advance() before your own code.
    */
    virtual void advance();

    //! Check whether an Entity needs to be tracked by this System,
    //! and if so, mark it to be added to the internal list.
    /*!
      This function should only be called once per Entity.
      You probably want to override checkRequirements instead of this functions, as it
      only deals with the Entity's requirements.

      \param e The entity to inspect.
    */
    void inspectEntity(Entity& e);

    //! Mark an Entity to be removed from the System on the next advance step.
    /*!
      \param e The Entity to remove.
    */
    void markToRemove(Entity& e);

protected:
    //! Add an Entity to the internal System structure.
    /*!
      This is where subclasses should handle actually adding the Entity.

      \param e The Entity to add.
    */
    virtual void internalAddEntity(Entity& e) = 0;

    //! Remove an Entity to the internal list.
    /*!
      This is where subclasses should handle actually removing the Entity.

      \param e The entity to remove.
    */
    virtual void internalRemoveEntity(Entity& e) = 0;

    //! Check whether an Entity meets the requirements to be added to this System.
    /*!
      \param e The entity to check.
      \return Whether the Entity matches this System's requirements.
    */
    virtual bool checkRequirements(const Entity& e) const = 0;

private:
    //! Mark an Entity to be added to the internal list on the advance step.
    /*!
      Buried in the private interface because only the System should really be adding Entities to itself.

      \param e The Entity to add.
    */
    void markToAdd(Entity& e);

    std::set<Entity*> toAdd;    //!< Entities to be added to the System on the advance step.
    std::set<Entity*> toRemove; //!< Entities to be removed from the System on the advance step.
};

