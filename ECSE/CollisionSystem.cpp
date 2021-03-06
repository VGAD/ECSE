#include <algorithm>
#include "TransformSystem.h"
#include "World.h"
#include "CollisionSystem.h"
#include "CollisionMath.h"
#include "CircleColliderComponent.h"
#include "LineColliderComponent.h"

namespace ECSE
{

void CollisionSystem::advance()
{
    SetSystem::advance();

    // Collisions before this time have either been dealt with or are invalid
    float startTime = 0.f;

    // Any changes which occurred due to collision response
    // Initially, consider all entities "changed"
    ColliderComponent::ChangeSet changes(getEntities());

    // Build caches for each Entity.
    std::vector<EntityCache> caches;
    for (auto& entity : changes)
    {
        caches.push_back(EntityCache(entity, this));
    }

    auto potentialCollisions = getPotentialCollisions(caches);

    while (!changes.empty())
    {
        // Determine time of each collision
        for (auto& pc : potentialCollisions)
        {
            // This collision has to do with an entity which is either unsolved or changed
            if (pc.time == -1.f ||
                changes.find(pc.first->entity) != changes.end() ||
                changes.find(pc.second->entity) != changes.end())
            {
                findCollisionTime(pc);
            }
        }
        changes.clear();

        // Sort by time
        std::sort(potentialCollisions.begin(), potentialCollisions.end());

        bool toBreak = false;

        // Actually carry out each collision
        for (auto& pc : potentialCollisions)
        {
            // Don't actually break until we've moved to a different time.
            // This avoids ignoring collisions if they have the same time.
            if (toBreak && pc.time != startTime) break;

            if (pc.time < startTime) continue;

            ColliderComponent::ChangeSet newChanges = resolve(pc);
            if (!newChanges.empty())
            {
                // Something changed, so we have to check for collisions again once this time is dealt with
                startTime = pc.time;
                changes.insert(newChanges.begin(), newChanges.end());
                toBreak = true;
            }
        }

        // Update caches
        if (!changes.empty())
        {
            for (auto& cache : caches)
            {
                if (changes.find(cache.entity) != changes.end())
                {
                    cache.update(this, startTime);
                }
            }
        }
    }
}

bool CollisionSystem::checkRequirements(const Entity& e) const
{
    if (!e.getComponent<TransformComponent>()) return false;

    // Need one of these
    if (e.getComponent<CircleColliderComponent>()) return true;
    if (e.getComponent<LineColliderComponent>()) return true;

    return false;
}

void CollisionSystem::added()
{
    transformSystem = world->getSystem<TransformSystem>();
    
    if (transformSystem == nullptr)
    {
        throw std::runtime_error("CollisionSystem requires a TransformSystem");
    }

    if (world->getSystemPosition<TransformSystem>() < world->getSystemPosition<CollisionSystem>())
    {
        throw std::runtime_error("CollisionSystem should be added before TransformSystem");
    }
}

sf::Vector2f CollisionSystem::getColliderPosition(const Entity& e) const
{
    auto collider = e.getComponent<ColliderComponent>();

    if (collider == nullptr)
    {
        throw std::runtime_error("Entity has no collider");
    }

    auto collOffset = collider->offset;
    rotate(collOffset, transformSystem->getGlobalAngle(e));

    return transformSystem->getGlobalPosition(e) + collOffset;
}

sf::Vector2f CollisionSystem::getNextColliderPosition(const Entity& e) const
{
    auto collider = e.getComponent<ColliderComponent>();

    if (collider == nullptr)
    {
        throw std::runtime_error("Entity has no collider");
    }

    auto collOffset = collider->offset;
    rotate(collOffset, transformSystem->getNextGlobalAngle(e));

    return transformSystem->getNextGlobalPosition(e) + collOffset;
}

std::vector<CollisionSystem::PotentialCollision> CollisionSystem::getPotentialCollisions(std::vector<EntityCache>& caches) const
{
    std::vector<PotentialCollision> collisions;

    // First entity in the collision
    auto cacheA = caches.begin();

    while (cacheA != caches.end())
    {
        // Second entity in the collision
        auto cacheB = std::next(cacheA);

        while (cacheB != caches.end())
        {
            // These two entities are able to collide
            collisions.push_back(PotentialCollision(&*cacheA, &*cacheB));

            ++cacheB;
        }

        ++cacheA;
    }

    return collisions;
}

// http://www.gamasutra.com/view/feature/131424/pool_hall_lessons_fast_accurate_.php?page=2
void CollisionSystem::findCollisionTime(PotentialCollision& pc) const
{
    auto colliderA = pc.first->collider;
    auto colliderB = pc.second->collider;

    bool aHasCollider = (colliderA && colliderA->enabled);
    bool bHasCollider = (colliderB && colliderB->enabled);

    auto typeA = pc.first->type;
    auto typeB = pc.second->type;

    // No colliders
    if (!aHasCollider || !bHasCollider)
    {
        pc.time = -1.f;
        return;
    }
    
    // If one of the start times is non-zero, then we're dealing with a collision in a smaller
    // time scale, e.g. if maxStartTime is 0.3, then the collision can only take place in the
    // last 0.7 units of our frame
    float startTimeA = pc.first->startTime;
    float startTimeB = pc.second->startTime;

    float maxStartTime = std::max(startTimeA, startTimeB);
    float timeScale = 1.f - maxStartTime;

    sf::Vector2f startA = pc.first->start;
    sf::Vector2f endA = pc.first->end;
    sf::Vector2f startB = pc.second->start;
    sf::Vector2f endB = pc.second->end;

    // If one start time is greater than the other, then we also need to interpolate the position
    // of the "earlier" object so they both start at the same time.
    if (startTimeA < startTimeB)
    {
        float lerpAmt = timeScale == 0.f ? 1.f : (startTimeB - startTimeA) / timeScale;
        lerpAmt = ECSE::clamp(0.f, 1.f, lerpAmt);
        startA = ECSE::lerp(startA, endA, lerpAmt);
    }
    else if (startTimeB < startTimeA)
    {
        float lerpAmt = timeScale == 0.f ? 1.f : (startTimeA - startTimeB) / timeScale;
        lerpAmt = ECSE::clamp(0.f, 1.f, lerpAmt);
        startB = ECSE::lerp(startB, endB, lerpAmt);
    }

    // Work with relative motion rather than absoute motion to reduce the
    // problem to one moving collider and one stationary
    sf::Vector2f moveVec = (endA - startA) - (endB - startB);

    // Circle-circle collision
    if (typeA == EntityCache::CIRCLE && typeB == EntityCache::CIRCLE)
    {
        auto circleA = dynamic_cast<CircleColliderComponent*>(pc.first->collider);
        auto circleB = dynamic_cast<CircleColliderComponent*>(pc.second->collider);

        circleCircle(startA, circleA->radius, startB, circleB->radius, moveVec,
                     pc.time, pc.normal);
    }
    // Circle-line collision
    else if (typeA == EntityCache::CIRCLE && typeB == EntityCache::LINE)
    {
        auto circleA = dynamic_cast<CircleColliderComponent*>(pc.first->collider);
        auto lineB = dynamic_cast<LineColliderComponent*>(pc.second->collider);

        circleLine(startA, circleA->radius, startB, startB + lineB->vec, moveVec,
                   pc.time, pc.normal);
    }
    // Line-circle collision
    else if (typeA == EntityCache::LINE && typeB == EntityCache::CIRCLE)
    {
        auto lineA = dynamic_cast<LineColliderComponent*>(pc.first->collider);
        auto circleB = dynamic_cast<CircleColliderComponent*>(pc.second->collider);

        circleLine(startB, circleB->radius, startA, startA + lineA->vec, -moveVec,
                   pc.time, pc.normal);
    }
    else
    {
        // No collision detection for this pair of types (yet?)
        pc.time = -1.f;
        return;
    }

    // If we started this collision check at a later time than 0.f, then we need to modify
    // the time value accordingly. Note that if maxStartTime is 0.f, nothing happens here.
    if (pc.time > 0.f)
        pc.time = maxStartTime + timeScale * pc.time;

    return;
}

ColliderComponent::ChangeSet CollisionSystem::resolve(const PotentialCollision& pc) const
{
    auto colliderA = pc.first->collider;
    auto colliderB = pc.second->collider;

    // May have been disabled in the middle of collision detection
    if (!colliderA->enabled || !colliderB->enabled) return {};

    // Build the collision
    float firstTime = pc.first->startTime;
    float secondTime = pc.second->startTime;

    auto collision = Collision(pc.first->entity,
                               pc.second->entity,
                               pc.time,

                               // We need to scale the time here to account for changing positions in mid-frame
                               transformSystem->getInterpGlobalPosition(
                                   *pc.first->entity,
                                   firstTime == 1.f ? 1.f : (pc.time - firstTime) / (1 - firstTime)
                               ),
                               transformSystem->getInterpGlobalPosition(
                                   *pc.second->entity,
                                   secondTime == 1.f ? 1.f : (pc.time - secondTime) / (1 - secondTime)
                               ),

                               pc.normal);
    
    ColliderComponent::ChangeSet changes;
    auto newChanges = colliderA->callCallbacks(collision);
    changes.insert(newChanges.begin(), newChanges.end());

    // Invert it for the second entity so it sees colliderB as itself
    collision.invert();

    newChanges = colliderB->callCallbacks(collision);
    changes.insert(newChanges.begin(), newChanges.end());

    return changes;
}

CollisionSystem::EntityCache::EntityCache(Entity* entity, CollisionSystem* cs)
    : entity(entity)
{
    setupCollider();
    update(cs, 0.f);
}

void CollisionSystem::EntityCache::update(CollisionSystem* cs, float time)
{
    startTime = time;
    start = cs->getColliderPosition(*entity);

    if (entity->getComponent<TransformComponent>()->isPositionDiscrete())
    {
        end = start;
    }
    else
    {
        end = cs->getNextColliderPosition(*entity);
    }
}

void CollisionSystem::EntityCache::setupCollider()
{
    auto circle = entity->getComponent<CircleColliderComponent>();
    if (circle != nullptr)
    {
        collider = circle;
        type = CIRCLE;
        return;
    }

    auto line = entity->getComponent<LineColliderComponent>();
    if (line != nullptr)
    {
        collider = line;
        type = LINE;
        return;
    }

    collider = nullptr;
}

}
