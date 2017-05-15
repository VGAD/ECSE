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

            // How far were we originally going to move?
            auto speed = ECSE::getMagnitude(tc->getLocalDeltaPosition());

            // Move to collision position
            tc->setLocalPosition(collision.position);

            auto line = collision.other->getComponent<ECSE::LineColliderComponent>();
            if (line)
            {
                auto lineHeading = ECSE::getHeading(line->vec);
                auto velocityHeading = ECSE::getHeading(velocity);

                // Mirror the velocity about the line
                auto newHeading = lineHeading + (lineHeading - velocityHeading);
                ECSE::setHeading(velocity, newHeading);
            }

            // Update next position based on velocity (using the remaining fraction of time)
            auto remainder = velocity;
            ECSE::normalize(remainder);
            remainder *= speed * (1 - collision.time);

            tc->setNextLocalPosition(collision.position + remainder);

            return { e };
        });
    }
};

}