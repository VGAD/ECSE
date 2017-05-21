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
    float lastHitTime;          //!< The last time instance this hit something.
    unsigned int hitCount;      //!< The number of times this has hit something in the same instance in time.

    //! The positions this has moved/deflected from this frame (other than its final position).
    std::vector<sf::Vector2f> trajectory;

    //! Called when this is attached to an Entity.
    virtual void attached(ECSE::Entity* e)
    {
        hitCount = 0;
        lastHitTime = 0;

        auto collider = e->getComponent<ECSE::CircleColliderComponent>();
        if (!collider)
        {
            throw std::runtime_error("BallComponent requires a CircleColliderComponent");
        }

        collider->addCallback([&, collider, e](const ECSE::Collision& collision) -> ECSE::ColliderComponent::ChangeSet
        {
            // A bit of a hack -- we're stuck inside something, so just ignore collisions.
            // This can happen if balls spawn inside of each other.
            if (lastHitTime == collision.time)
            {
                ++hitCount;

                if (hitCount > 5)
                {
                    return {};
                }
            }
            else
            {
                lastHitTime = collision.time;
                hitCount = 1;
            }

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

            trajectory.push_back(collision.position);

            return { e };
        });
    }
};

}