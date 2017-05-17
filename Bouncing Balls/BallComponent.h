#pragma once

#include "SFML/System.hpp"
#include "ECSE/Component.h"
#include "ECSE/LineColliderComponent.h"
#include "ECSE/VectorMath.h"

namespace BouncingBalls
{

//! A Component which stores physics data for a ball.
class BallComponent : public ECSE::Component
{
public:
    sf::Vector2f direction;     //!< Current direction vector.
    float speed;                //!< Movement speed. Updated by BallSystem each frame.

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

            sf::Vector2f tangent = collision.normal;
            ECSE::rotate90(tangent);

            // Mirror the velocity about the tangent
            auto tanHeading = ECSE::getHeading(tangent);
            auto dirHeading = ECSE::getHeading(direction);
            auto newHeading = tanHeading + (tanHeading - dirHeading);
            ECSE::setHeading(direction, newHeading);
            ECSE::normalize(direction);

            // Update next position based on velocity (using the remaining fraction of time)
            auto remainder = direction;
            remainder *= speed * (1 - collision.time);
            tc->setDeltaPosition(remainder);

            return { e };
        });
    }
};

}