#include "ECSE/Engine.h"
#include "ECSE/TransformSystem.h"
#include "CollisionTestA.h"
#include "TestUtil.h"
#include "CollisionSystem.h"
#include "CollisionDebugSystem.h"
#include "CircleColliderComponent.h"
#include <random>

namespace Magnaut {

ECSE::Entity::ID CollisionTestStateA::createFlashingCircle(sf::Vector2f start, sf::Vector2f end, float radius, bool discrete)
{
    ECSE::Entity::ID id = createCircle(world, start, end, radius, discrete);
    
    ECSE::Entity* entity = world.getEntity(id);

    auto collider = entity->getComponent<CircleColliderComponent>();
    collider->addCallback([&, collider, entity](const Collision& collision) -> ColliderComponent::ChangeSet
    {
        // Ignore collisions
        if (getEngine()->getTimeSteps() % 120 < 60)
        {
            return{};
        }

        // Disable collisions after this point
        collider->enabled = false;

        return{ entity };
    });

    return id;
}

void CollisionTestStateA::advance()
{
    // Re-enable colliders
    for (auto entity : world.getEntities())
    {
        entity->getComponent<CircleColliderComponent>()->enabled = true;
    }

    WorldState::advance();
}

CollisionTestStateA::CollisionTestStateA(ECSE::Engine* engine)
    : WorldState(engine)
{
    world.addSystem<CollisionDebugSystem>();
    world.addSystem<CollisionSystem>();
    world.addSystem<ECSE::TransformSystem>();

    // One stationary
    createFlashingCircle(sf::Vector2f(185.f, 30.f), sf::Vector2f(185.f, 30.f), 15.f);
    createFlashingCircle(sf::Vector2f(155.f, 20.f), sf::Vector2f(205.f, 55.f), 10.f);

    // Perpendicular
    createFlashingCircle(sf::Vector2f(100.f, 25.f), sf::Vector2f(100.f, 75.f), 8.f);
    createFlashingCircle(sf::Vector2f(75.f, 50.f), sf::Vector2f(125.f, 50.f), 8.f);

    // Both moving at an angle
    createFlashingCircle(sf::Vector2f(15.f, 40.f), sf::Vector2f(63.f, 10.f), 8.f);
    createFlashingCircle(sf::Vector2f(75.f, 15.f), sf::Vector2f(10.f, 25.f), 4.f);

    // Discrete jump
    createFlashingCircle(sf::Vector2f(240.f, 35.f), sf::Vector2f(290.f, 35.f), 10.f);
    createFlashingCircle(sf::Vector2f(235.f, 65.f), sf::Vector2f(300.f, 15.f), 5.f, true);

    // Near miss
    createFlashingCircle(sf::Vector2f(350.f, 10.f), sf::Vector2f(370.f, 75.f), 5.f);
    createFlashingCircle(sf::Vector2f(325.f, 25.f), sf::Vector2f(375.f, 25.f), 5.f);

    // Add some random circles
    std::default_random_engine re(5150);
    std::uniform_real_distribution<float> rand01(0, 1);
    std::uniform_real_distribution<float> randRadius(2.f, 10.f);

    float minY = 100.f;

    for (int i = 0; i < 20; ++i)
    {
        float radius = randRadius(re);
        float x1 = radius + rand01(re) * (400.f - 2 * radius);
        float y1 = minY + radius + rand01(re) * (300.f - minY - 2 * radius);
        float x2 = radius + rand01(re) * (400.f - 2 * radius);
        float y2 = minY + radius + rand01(re) * (300.f - minY - 2 * radius);
        bool discrete = rand01(re) <= 0.05;

        createFlashingCircle(sf::Vector2f(x1, y1), sf::Vector2f(x2, y2), radius, discrete);
    }
}

}
