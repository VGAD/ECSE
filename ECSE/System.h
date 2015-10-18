#pragma once

#include <set>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include "Entity.h"

namespace ECSE
{

class World;

//! An interface which maintains a list of Entities and performs operations on their Components.
class System
{
public:
    //! Construct the System.
    /*!
    * \param world The World this System will belong to.
    */
    explicit System(World* world);

    //! Destroy the System.
    virtual ~System();

    //! Return whether an Entity is already in the System.
    /*!
    * \param e The Entity to check.
    * \return Whether the Entity is already in the System's internal structure.
    */
    virtual bool hasEntity(const Entity& e) const = 0;

    //! Called when all Systems have been added to the world.
    /*!
    * Override this to get references to other Systems.
    */
    virtual void added();

    //! Called on an update step.
    /*!
    * Each Entity should react to its current states, then prepare its next state.
    * 
    * \param deltaTime The amount of elapsed time to simulate.
    */
    virtual void update(sf::Time deltaTime);

    //! Called on a render step.
    /*!
    * Each Entity should be drawn, interpolated between its current and next state.
    * 
    * \param alpha The amount of interpolation between the two states.
    * \param renderTarget The RenderTarget to draw to.
    */
    virtual void render(float alpha, sf::RenderTarget& renderTarget);

    //! Called on an advance step.
    /*!
    * First, updates the internal list of Entities.
    * Next, each Entity should update its current state to advance the simulation.
    */
    virtual void advance();

    //! Add new entities and remove old ones.
    /*!
    * Called when the system is added and after each update step.
    */
    virtual void addAndRemove();

    //! Check whether an Entity needs to be tracked by this System, and if so, mark it to be added on the next advance.
    /*!
    * This function should only be called once per Entity.
    * You probably want to override checkRequirements instead of this
    * function, as it deals specifically with the Entity's requirements
    * to be added.
    * 
    * \param e The entity to inspect.
    */
    void inspectEntity(Entity& e);

    //! Mark an Entity to be removed from the System on the next call to addAndRemove.
    /*!
    * \param e The Entity to remove.
    */
    virtual void markToRemove(Entity& e);

    //! Returns the System's world.
    /*!
    * \return A pointer to the System's world.
    */
    World* getWorld() const;

protected:
    //! Add an Entity to the internal System structure.
    /*!
    * This is where subclasses should handle actually adding the Entity.
    * 
    * \param e The Entity to add.
    */
    virtual void internalAddEntity(Entity& e) = 0;

    //! Remove an Entity from the internal System structure.
    /*!
    * This is where subclasses should handle actually removing the Entity.
    * 
    * \param e The Entity to remove.
    */
    virtual void internalRemoveEntity(Entity& e) = 0;

    //! Check whether an Entity meets the requirements to be added to this System.
    /*!
    * \param e The Entity to check.
    * \return Whether the Entity matches this System's requirements.
    */
    virtual bool checkRequirements(const Entity& e) const = 0;


    // Data
    World* world;   //!< The World to which this System belongs.

private:
    //! Mark an Entity to be added to the internal list on the advance step.
    /*!
    * Buried in the private interface because only the System should really be adding Entities to itself.
    * 
    * \param e The Entity to add.
    */
    void markToAdd(Entity& e);

    std::set<Entity*> toAdd;    //!< Entities to be added to the System on the next call to addAndRemove.
    std::set<Entity*> toRemove; //!< Entities to be removed from the System on the next call to addAndRemove.
};

}
