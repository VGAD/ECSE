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

    //! Called on an advance
    /*!
    * Balls' last hits are reset.
    */
    virtual void advance() override;

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

    //! Whether to draw the trajectory trails of balls.
    bool drawTrails = true;

private:
    //! The RenderTarget to which this will draw.
    sf::RenderTarget& renderTarget;

    //! Font used for drawing.
    sf::Font font;

    //! Text used for drawing.
    sf::Text text;

    //! The circle shape reused in drawing.
    sf::CircleShape circleShape;

    //! The TransformSystem of this System's World.
    ECSE::TransformSystem* transformSystem;

    //! The CollisionSystem of this System's World.
    ECSE::CollisionSystem* collisionSystem;

    //! The input manager.
    ECSE::InputManager* inputMan;

    //! Speed of ball movement.
    float ballSpeed = 1000.f;

    //! Rate of change for ballSpeed when input key is pressed.
    float ballSpeedChange = 25.f;


    //! Draw the trajectory of a ball.
    /*!
    * \param e The Entity.
    */
    void drawBall(const ECSE::Entity& e);

    //! Draw a line.
    /*!
    * \param start The start point of the line.
    * \param end The end point of the line.
    * \param color The line color.
    */
    void drawLine(const sf::Vector2f& start, const sf::Vector2f& end, const sf::Color& color);
};

}