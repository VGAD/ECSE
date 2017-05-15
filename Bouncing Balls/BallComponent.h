#pragma once

#include "SFML/System.hpp"
#include "ECSE/Component.h"

namespace BouncingBalls
{

//! A Component which stores physics data for a ball.
class BallComponent : public ECSE::Component
{
public:
    sf::Vector2f velocity;  //!< Current velocity in pixels per second.

    //! Called when this is attached to an Entity.
    virtual void attached(ECSE::Entity* e)
    {
        auto collider = e->getComponent<ECSE::CircleColliderComponent>();
        if (!collider)
        {
            throw std::runtime_error("BallComponent requires a CircleColliderComponent");
        }

        collider->addCallback([&, collider, e](const ECSE::Collision& collision) -> ECSE::ColliderComponent::ChangeSet
        {
            auto tc = e->getComponent<ECSE::TransformComponent>();

            // Move to collision position
            tc->setLocalPosition(collision.position);

            velocity = sf::Vector2f();

            return { e };
        });
    }
};

}