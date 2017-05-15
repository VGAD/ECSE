#include "ECSE/TransformSystem.h"
#include "ECSE/CircleColliderComponent.h"
#include "ECSE/LineColliderComponent.h"
#include "TestUtil.h"

ECSE::Entity::ID createCircle(ECSE::World& world, sf::Vector2f start, sf::Vector2f end,
                              float radius, bool discrete)
{
    ECSE::Entity::ID id = world.createEntity();
    ECSE::TransformComponent* tc = world.attachComponent<ECSE::TransformComponent>(id);
    tc->setLocalPosition(start, false);
    tc->setNextLocalPosition(end, discrete);
    tc->enabled = false;

    auto collider = world.attachComponent<ECSE::CircleColliderComponent>(id);
    collider->radius = radius;

    world.registerEntity(id);

    return id;
}

ECSE::Entity::ID createLine(ECSE::World& world, sf::Vector2f start, sf::Vector2f end,
                            sf::Vector2f vec, bool discrete)
{
    ECSE::Entity::ID id = world.createEntity();
    ECSE::TransformComponent* tc = world.attachComponent<ECSE::TransformComponent>(id);
    tc->setLocalPosition(start, false);
    tc->setNextLocalPosition(end, discrete);
    tc->enabled = false;

    auto collider = world.attachComponent<ECSE::LineColliderComponent>(id);
    collider->vec = vec;

    world.registerEntity(id);

    return id;
}