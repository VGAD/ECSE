#pragma once

#include "ECSE/Specialization.h"
#include "ECSE/TransformComponent.h"
#include "ECSE/InputManager.h"

namespace ReplayDemo
{

//! A controllable moving ball.
class ControllableBall : public ECSE::Specialization
{
    //! Initialize the ball.
    void init() override;

    //! Update the ball's position.
    void update(sf::Time deltaTime) override;

    //! Render the ball.
    void render(float alpha, sf::RenderTarget &renderTarget) override;

private:
    //! Movement speed (pixels/second).
    const float moveSpeed = 600.f;

    //! The transform component.
    ECSE::TransformComponent *transform;

    //! The input manager.
    ECSE::InputManager* inputMan;

    //! The circle shape to render this.
    sf::CircleShape circle;
};

}