#pragma once

#include "SFML/System/Time.hpp"
#include "SFML/Graphics/RenderTarget.hpp"
#include "Entity.h"

namespace ECSE
{

class World;

//! A class to which the SpecializationComponent/SpecializationSystem defers.
class Specialization
{
    friend class SpecializationSystem;

public:
    //! Destroy the Specialization.
    virtual ~Specialization() {}

    //! Called when the Specialization is ready to interact with the World and other Entities.
    virtual void init() {}

    //! Called on an update step.
    /*!
    * The Specialization should react to its current states, then prepare its next state.
    *
    * \param deltaTime The amount of elapsed time to simulate.
    */
    virtual void update(sf::Time deltaTime) {}

    //! Called on an advance step.
    /*!
    * The Specialization should update its current state to advance the simulation.
    */
    virtual void advance() {}

    //! Called on a render step.
    /*!
    * The Specialization should be drawn, interpolated between its current and next state.
    *
    * \param alpha The amount of interpolation between the two states.
    * \param renderTarget The RenderTarget to draw to.
    */
    virtual void render(float alpha, sf::RenderTarget& renderTarget) {}

protected:
    Entity* entity = nullptr;   //!< The entity to which this is attached.
    World* world = nullptr;     //!< The world in which this exists.
};

}
