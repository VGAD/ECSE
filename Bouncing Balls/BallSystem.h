#pragma once

#include "ECSE/Engine.h"
#include "ECSE/World.h"
#include "ECSE/SetSystem.h"
#include "ECSE/CollisionSystem.h"
#include "ECSE/CircleColliderComponent.h"
#include "ECSE/CollisionSystem.h"
#include "ECSE/TransformComponent.h"

namespace BouncingBalls
{

//! Manages the movement of bouncing balls and renders them.
class BallSystem :
    public ECSE::SetSystem
{
public:
    //! Construct the BallSystem.
    explicit BallSystem(ECSE::World* world);

    //! Called when all Systems have been added to the world.
    /*!
    * Override this to get references to other Systems.
    */
    virtual void added() override;

    //! Called on an update step.
    /*!
    * Balls' next positions are updated.
    */
    virtual void update(sf::Time deltaTime) override;

    //! Called on a render step.
    /*!
    * Each Entity should be drawn, interpolated between its current and next state.
    *
    * \param alpha The amount of interpolation between the two states.
    * \param renderTarget The RenderTarget to draw to.
    */
    virtual void render(float alpha, sf::RenderTarget& renderTarget) override;

    //! Check whether an Entity meets the requirements to be added to this System.
    /*!
    * \param e The Entity to check.
    * \return Whether the Entity matches this System's requirements.
    */
    bool checkRequirements(const ECSE::Entity& e) const override;

private:
    //! Draw a ball entity.
    /*!
    * \param e The Entity.
    */
    void drawBall (const ECSE::Entity& e);

    //! The RenderTarget to which this will draw.
    sf::RenderTarget& renderTarget;

    //! The circle shape reused in drawing.
    sf::CircleShape circleShape;

    //! The TransformSystem of this System's World.
    ECSE::TransformSystem* transformSystem;

    //! The CollisionSystem of this System's World.
    ECSE::CollisionSystem* collisionSystem;
};

}