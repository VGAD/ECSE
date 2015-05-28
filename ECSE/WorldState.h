#pragma once

#include "State.h"
#include "World.h"

namespace ECSE
{

//! A State which maintains a single World.
class WorldState : public State
{
public:
    //! Construct the WorldState.
    /*!
    * \param engine The State's engine.
    */
    explicit WorldState(Engine* engine);

    //! Destroy the WorldState.
    virtual ~WorldState();

    //! Perform the update step.
    /*!
    * \param deltaTime The time elapsed in this update.
    */
    virtual void update(sf::Time deltaTime) override;

    //! Perform the advance step.
    virtual void advance() override;

    //! Perform the render step.
    /*!
    * \param alpha The amount of interpolation in this render.
    * \param renderTarget The RenderTarget to draw to.
    */
    virtual void render(float alpha, sf::RenderTarget& renderTarget) override;

    //! Get the name of this State class.
    /*!
    * \return The name of the class.
    */
    virtual std::string getName() override;

protected:
    World world;    //!< The contained world.
};

}
