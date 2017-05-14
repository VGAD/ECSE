#pragma once

#include "SetSystem.h"
#include "TransformSystem.h"
#include "ColliderComponent.h"

namespace ECSE
{

//! A System that handles collision detection and response.
class CollisionSystem :
    public SetSystem
{
public:
    //! Construct the CollisionSystem.
    explicit CollisionSystem(World* world)
        : SetSystem(world)
    {
    }

    //! Called on an advance step.
    /*!
    * Detects collisions for each collider in the system.
    */
    void advance() override;

    //! Check whether an Entity meets the requirements to be added to this System.
    /*!
    * \param e The Entity to check.
    * \return Whether the Entity matches this System's requirements.
    */
    bool checkRequirements(const Entity& e) const override;

    //! Called when all Systems have been added to the world.
    /*!
    * Override this to get references to other Systems.
    */
    virtual void added() override;

    //! Get the global position of an Entity's ColliderComponent.
    /*!
    * \param e The entity.
    */
    sf::Vector2f getColliderPosition(const Entity& e) const;

    //! Get the next global position of an Entity's ColliderComponent.
    /*!
    * \param e The entity.
    */
    sf::Vector2f getNextColliderPosition(const Entity& e) const;

private:
    ///////
    // Data

    //! The TransformSystem of this world.
    TransformSystem* transformSystem;

    //! Stores data about an Entity so we can avoid using iterators (which slow down debug mode a lot).
    struct EntityCache
    {
        Entity* entity;               //!< The Entity.
        ColliderComponent* collider;        //!< The Entity's ColliderComponent.
        sf::Vector2f start;                 //!< The Entity's start position.
        sf::Vector2f end;                   //!< The Entity's next position.

        enum ColliderType
        {
            NONE,
            CIRCLE,
            LINE
        } type; //!< The type of the Entity's collider.

        //! Construct an EntityCache.
        /*!
        * \param entity The Entity whose data we want to cache.
        * \param cs The CollisionSystem which holds the Entity.
        */
        EntityCache(Entity* entity, CollisionSystem* cs);

        //! Update the cache when the Entity is changed.
        /*!
        * \param cs The CollisionSystem which holds the Entity.
        */
        void update(CollisionSystem* cs);

    private:
        //! Get the collider pointer and type.
        void setupCollider();
    };

    //! Stores data about a collision which may happen and the time at which it will happen.
    struct PotentialCollision
    {
        EntityCache* first;     //!< The first Entity in the collision.
        EntityCache* second;    //!< The second Entity in the collision.
        float time;             //!< The time at which the collision will occur, or <0 if it won't occur.

        //! Construct a PotentialCollision.
        PotentialCollision(EntityCache* first, EntityCache* second, float time)
            : first(first), second(second), time(time)
        {
        }

        //! Compare based on collision time.
        /*!
        * \param other The other PotentialCollision.
        * \return Whether this will happen first.
        */
        bool operator<(const PotentialCollision& other) const
        {
            return time < other.time;
        }
    };

    //! Hash functor for PotentialCollisions.
    struct PCHash
    {
        //! Get the hash value of a PotentialCollision.
        /*!
        * \param pc The PotentialCollision.
        * \return The has value.
        */
        std::size_t operator()(const PotentialCollision& pc) const
        {
            return reinterpret_cast<size_t>(pc.first) ^ reinterpret_cast<size_t>(pc.second);
        }
    };


    ////////////
    // Functions

    //! Update all potential collision pairs.
    /*!
    * \param caches The cached Entity data.
    * \return A vector of PotentialCollisions with times of -1.f to indicate that they haven't been solved.
    */
    std::vector<PotentialCollision> getPotentialCollisions(std::vector<EntityCache>& caches) const;

    //! Find the time of collision for a potential collision between two entities.
    /*!
    * Sets the time value of the PotentialCollision to the found value if there was
    * a collision.
    * \param pc The PotentialCollision.
    * \return The time at which the collision occurred, or <0 if no collision.
    */
    float findCollisionTime(const PotentialCollision& pc) const;

    //! Notify colliders of a collision that actually happened.
    /*!
    * \param pc The PotentialCollision.
    * \return Any Entities which were changed by the collision.
    */
    ColliderComponent::ChangeSet resolve(const PotentialCollision& pc) const;
};

}
