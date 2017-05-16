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
            // We're stuck inside of something, so don't keep hitting it
            if (collision.time == 0 && collision.other == lastHit)
            {
                return { };
            }

            auto tc = e->getComponent<ECSE::TransformComponent>();

            // How far were we originally going to move?
            auto speed = ECSE::getMagnitude(tc->getLocalDeltaPosition());

            // Move to collision position
            tc->setLocalPosition(collision.position);

            sf::Vector2f tangent;

            auto line = collision.other->getComponent<ECSE::LineColliderComponent>();
            if (line)
            {
                tangent = line->vec;
            }
            else
            {
                auto circle = collision.other->getComponent<ECSE::CircleColliderComponent>();
                if (circle)
                {
                    // Tangent is 90 degrees to the difference in position between the circles
                    auto diff = collision.position - collision.otherPosition;
                    ECSE::rotate90(diff);
                    tangent = diff;
                }
            }

            // Mirror the velocity about the tangent
            auto tanHeading = ECSE::getHeading(tangent);
            auto velHeading = ECSE::getHeading(velocity);
            auto newHeading = tanHeading + (tanHeading - velHeading);
            ECSE::setHeading(velocity, newHeading);

            // Update next position based on velocity (using the remaining fraction of time)
            auto remainder = velocity;
            ECSE::normalize(remainder);
            remainder *= speed * (1 - collision.time);

            tc->setNextLocalPosition(collision.position + remainder);

            lastHit = collision.other;

            return { e };
        });
    }

private:
    ECSE::Entity *lastHit;  //!< The last thing this ran into.
};

}