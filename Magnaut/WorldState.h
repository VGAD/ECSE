#pragma once

#include <memory>
#include "State.h"
#include "World.h"

//! A State which maintains a single World.
class WorldState : public State
{
public:
    //! Construct the WorldState.
    WorldState();

    //! Destroy the WorldState.
    virtual ~WorldState();

    //! Perform the update step.
    /*!
    * \param deltaTime The time elapsed in this update.
    */
    virtual void update(sf::Time deltaTime);

    //! Perform the advance step.
    virtual void advance();

    //! Perform the render step.
    /*!
    * \param alpha The amount of interpolation in this render.
    */
    virtual void render(float alpha);

    //! Get the name of this State class.
    /*!
    * \return The name of the class.
    */
    virtual const std::string getName();

protected:
    World world;    //<! The contained world.
};