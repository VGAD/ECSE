#include "ECSE/Engine.h"
#include "ECSE/TransformSystem.h"
#include "ECSE/CollisionSystem.h"
#include "ECSE/CircleColliderComponent.h"
#include "ECSE/LineColliderComponent.h"
#include "LineRenderSystem.h"
#include "BouncingBalls.h"
#include "BallComponent.h"
#include "BallSystem.h"
#include "Bindings.h"

namespace BouncingBalls
{

BouncingBallsState::BouncingBallsState(ECSE::Engine* engine)
    : WorldState(engine)
{
    world.addSystem<BallSystem>();
    world.addSystem<ECSE::CollisionSystem>();
    world.addSystem<ECSE::TransformSystem>();
    world.addSystem<LineRenderSystem>();

    // Create walls around the world
    createWall(sf::Vector2f(0.f, 0.f), sf::Vector2f(800.f, 0.f));
    createWall(sf::Vector2f(800.f, 0.f), sf::Vector2f(0.f, 600.f));
    createWall(sf::Vector2f(800.f, 600.f), sf::Vector2f(-800.f, 0.f));
    createWall(sf::Vector2f(0.f, 600.f), sf::Vector2f(0.f, -600.f));

    // Create cross in the center
    createWall(sf::Vector2f(400.f, 150.f), sf::Vector2f(0.f, 300.f));
    createWall(sf::Vector2f(250.f, 300.f), sf::Vector2f(300.f, 0.f));

    rng.seed(rng.default_seed);

    createRandomBall();
}

//! Update the state.
void BouncingBallsState::advance()
{
    WorldState::advance();

    // Add/remove balls
    int ballChange = engine->inputManager.getIntValue(Bindings::ChangeBallCount);

    if (ballChange != 0)
    {
        // Only change ball count if the button was either just pressed or has been held for a few steps
        // Rate will increase after holding for a while
        if (ballHoldCount % (ballHoldCount > ballSpeedupTime ? fastBallWaitTime : ballWaitTime) == 0)
        {
            if (ballChange < 0 && balls.size() > minBalls)
            {
                world.destroyEntity(balls.back());
                balls.pop_back();
            }

            if (ballChange > 0 && balls.size() < maxBalls)
            {
                createRandomBall();
            }
        }

        ++ballHoldCount;
    }
    else
    {
        ballHoldCount = 0;
    }

    // Toggle ball trails
    if (engine->inputManager.getIntDelta(Bindings::ToggleTrails) == 1)
    {
        auto bs = world.getSystem<BallSystem>();
        bs->drawTrails = !bs->drawTrails;
    }
}

// Create a bouncing ball
void BouncingBallsState::createRandomBall()
{
    auto id = world.createEntity();

    // Pick a random direction
    std::uniform_real_distribution<float> angleDist(0, ECSE::twoPi);
    sf::Vector2f direction = sf::Vector2f(1.f, 0.f);
    ECSE::setHeading(direction, angleDist(rng));

    // Pick a random radius
    std::uniform_real_distribution<float> radiusDist(minBallSize, maxBallSize);
    float radius = radiusDist(rng);
    float radiusSqr = radius * radius;

    // Pick a random position
    std::uniform_real_distribution<float> xDist(1.f + radius, 799.f - radius);
    std::uniform_real_distribution<float> yDist(1.f + radius, 599.f - radius);

    // Keep trying until we find an unblocked position
    sf::Vector2f pos;
    bool blocked = true;
    while (blocked)
    {
        pos = sf::Vector2f(xDist(rng), yDist(rng));
        blocked = false;

        // Check other balls
        for (auto id : balls)
        {
            auto ent = world.getEntity(id);
            assert(ent);

            auto coll = ent->getComponent<ECSE::CircleColliderComponent>();
            assert(coll);

            auto tc = ent->getComponent<ECSE::TransformComponent>();
            assert(tc);

            auto otherPos = tc->getLocalPosition();

            // These balls overlap, so we need a new position
            if (ECSE::getMagnitude(otherPos - pos) <= radius + coll->radius)
            {
                blocked = true;
                break;
            }
        }
    }

    createBall(pos, direction, radius);
}

void BouncingBallsState::createBall(sf::Vector2f start, sf::Vector2f direction, float radius)
{
    auto id = world.createEntity();
    auto* tc = world.attachComponent<ECSE::TransformComponent>(id);
    tc->setLocalPosition(start, false);

    auto collider = world.attachComponent<ECSE::CircleColliderComponent>(id);
    collider->radius = radius;

    auto bc = world.attachComponent<BallComponent>(id);
    ECSE::normalize(direction);
    bc->direction = direction;

    world.registerEntity(id);

    balls.push_back(id);
}

void BouncingBallsState::createWall(sf::Vector2f start, sf::Vector2f vec)
{
    ECSE::Entity::ID id = world.createEntity();
    ECSE::TransformComponent* tc = world.attachComponent<ECSE::TransformComponent>(id);
    tc->setLocalPosition(start, false);

    auto collider = world.attachComponent<ECSE::LineColliderComponent>(id);
    collider->vec = vec;

    world.registerEntity(id);
}

}