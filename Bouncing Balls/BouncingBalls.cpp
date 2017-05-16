#include "ECSE/Engine.h"
#include "ECSE/TransformSystem.h"
#include "ECSE/CollisionSystem.h"
#include "ECSE/CollisionDebugSystem.h"
#include "ECSE/CircleColliderComponent.h"
#include "ECSE/LineColliderComponent.h"
#include "BouncingBalls.h"
#include "BallComponent.h"
#include "BallSystem.h"
#include <random>

namespace BouncingBalls
{

ECSE::Entity::ID createBall(ECSE::World& world, sf::Vector2f start, sf::Vector2f velocity, float radius)
{
    auto id = world.createEntity();
    auto* tc = world.attachComponent<ECSE::TransformComponent>(id);
    tc->setLocalPosition(start, false);

    auto collider = world.attachComponent<ECSE::CircleColliderComponent>(id);
    collider->radius = radius;

    auto bc = world.attachComponent<BallComponent>(id);
    bc->velocity = velocity;

    world.registerEntity(id);

    return id;
}

ECSE::Entity::ID createWall(ECSE::World& world, sf::Vector2f start, sf::Vector2f vec)
{
    ECSE::Entity::ID id = world.createEntity();
    ECSE::TransformComponent* tc = world.attachComponent<ECSE::TransformComponent>(id);
    tc->setLocalPosition(start, false);

    auto collider = world.attachComponent<ECSE::LineColliderComponent>(id);
    collider->vec = vec;

    world.registerEntity(id);

    return id;
}

BouncingBallsState::BouncingBallsState(ECSE::Engine* engine)
    : WorldState(engine)
{
    world.addSystem<ECSE::CollisionSystem>();
    world.addSystem<ECSE::TransformSystem>();
    world.addSystem<BallSystem>();

    // Create walls around the world
    createWall(world, sf::Vector2f(0.f, 0.f), sf::Vector2f(800.f, 0.f));
    createWall(world, sf::Vector2f(800.f, 0.f), sf::Vector2f(0.f, 600.f));
    createWall(world, sf::Vector2f(800.f, 600.f), sf::Vector2f(-800.f, 0.f));
    createWall(world, sf::Vector2f(0.f, 600.f), sf::Vector2f(0.f, -600.f));

    createBall(world, sf::Vector2f(200.f, 300.f), sf::Vector2f(-300.f, -300.f), 40.f);
    createBall(world, sf::Vector2f(600.f, 300.f), sf::Vector2f(300.f, 300.f), 40.f);
}

}