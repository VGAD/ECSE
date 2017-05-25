#pragma once

#include "SFML/System.hpp"
#include "Entity.h"
#include "Component.h"
#include <vector>
#include <functional>

namespace ECSE
{

//! Represents a collision between two Entities.
struct Collision
{
    Entity* self;                   //!< The Entity that collided.
    Entity* other;                  //!< The Entity that self collided with.
    float time;                     //!< The inter-frame time at which the collision occurred (between 0 and 1).
    sf::Vector2f position;          //!< The position of this Entity when the collision occurred.
    sf::Vector2f otherPosition;     //!< The position of the other Entity when the collision occurred.
    sf::Vector2f normal;            //!< The normal of the collision (direction is from self to other).

    //! Construct a Collision.
    Collision(Entity* self, Entity* other, float time, sf::Vector2f position, sf::Vector2f otherPosition, sf::Vector2f normal)
        : self(self), other(other), time(time), position(position), otherPosition(otherPosition), normal(normal)
    {
    }

    //! Invert the collision (i.e. self becomes other and vice-versa).
    inline void invert()
    {
        std::swap(self, other);
        std::swap(position, otherPosition);
        normal = -normal;
    }
};

//! A Component which stores collider and collision data.
class ColliderComponent : public Component
{
public:
    //! The collider's offset from the object's center.
    sf::Vector2f offset;

    //! A set of Entities that were changed by a collision.
    typedef std::set<Entity*> ChangeSet;

    //! Collision callback type.
    /*!
    * The function takes a reference to Collision data and returns
    * a ChangeSet of any Entities which had their collision or transform
    * data changed.
    *
    * If any Entities were changed, then collisions occurring after this
    * point will be recalculated. If an Entity is marked as changed, then
    * TransformComponent's current position should be set to the new position
    * as a result of the collision (e.g. the position passed in the Collision
    * structure). You may also change the TransformComponent's next position,
    * which still represents the position at the end of the frame (i.e. time
    * = 1).
    */
    typedef std::function<ChangeSet(const Collision&)> CallbackType;

    //! Adds a callback function which will be called when a collision occurs.
    /*!
    * \param fn The function to call. See CallbackType for more information.
    */
    inline void addCallback(CallbackType fn)
    {
        callbacks.push_back(fn);
    }

    //! Call the callback functions.
    /*!
    * \param collision The collision to handle.
    * \return Any Entities which were changed.
    */
    inline ChangeSet callCallbacks(const Collision& collision)
    {
        ChangeSet changes;

        for (auto& fn : callbacks)
        {
            auto newChanges = fn(collision);
            changes.insert(newChanges.begin(), newChanges.end());
        }

        return changes;
    };

private:
    std::vector<CallbackType> callbacks;    //! Callback functions for when collisions occur.
};

}
