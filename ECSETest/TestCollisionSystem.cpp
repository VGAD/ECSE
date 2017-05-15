#include "gtest/gtest.h"
#include "ECSE/World.h"
#include "ECSE/CollisionSystem.h"
#include "ECSE/CircleColliderComponent.h"
#include "ECSE/LineColliderComponent.h"
#include <functional>

class CollisionDebugComponent : public ECSE::Component
{
public:
    ECSE::ColliderComponent::ChangeSet onCollide(const ECSE::Collision& collision)
    {
        collisions.push_back(collision);

        if (disableOnHit)
        {
            // Disable all colliders with some ugly RTTI
            for (auto pair : collision.self->getComponents())
            {
                auto component = pair.second;

                if (dynamic_cast<ECSE::ColliderComponent*>(component) != nullptr)
                {
                    component->enabled = false;
                }
            }

            return {collision.self};
        }

        return {};
    }

    std::vector<ECSE::Collision> collisions;
    bool disableOnHit = false;
};

ECSE::Entity::ID createMovingEntity(ECSE::World& world, sf::Vector2f start, sf::Vector2f end, bool discrete = false)
{
    ECSE::Entity::ID id = world.createEntity();
    ECSE::TransformComponent* tc = world.attachComponent<ECSE::TransformComponent>(id);
    tc->setLocalPosition(start, false);
    tc->setNextLocalPosition(end, discrete);
    tc->enabled = false;

    return id;
}

class CollisionSystemTest : public ::testing::Test
{
public:
    CollisionSystemTest()
        : world(nullptr)
    {
    }

    void SetUp() override
    {
        system = world.addSystem<ECSE::CollisionSystem>();
        world.addSystem<ECSE::TransformSystem>();
    }

    CollisionDebugComponent* createCircle(sf::Vector2f start, sf::Vector2f end, float radius, bool discrete = false, sf::Vector2f offset = sf::Vector2f())
    {
        using namespace std::placeholders;

        ECSE::Entity::ID id = createMovingEntity(world, start, end, discrete);

        auto debugComponent = world.attachComponent<CollisionDebugComponent>(id);

        auto collider = world.attachComponent<ECSE::CircleColliderComponent>(id);
        collider->radius = radius;
        collider->offset = offset;
        collider->addCallback(std::bind(&CollisionDebugComponent::onCollide, debugComponent, _1));

        world.registerEntity(id);

        return debugComponent;
    }

    CollisionDebugComponent* createLine(sf::Vector2f start, sf::Vector2f end, sf::Vector2f vec, bool discrete = false, sf::Vector2f offset = sf::Vector2f())
    {
        using namespace std::placeholders;

        ECSE::Entity::ID id = createMovingEntity(world, start, end, discrete);

        auto debugComponent = world.attachComponent<CollisionDebugComponent>(id);

        auto collider = world.attachComponent<ECSE::LineColliderComponent>(id);
        collider->vec = vec;
        collider->offset = offset;
        collider->addCallback(std::bind(&CollisionDebugComponent::onCollide, debugComponent, _1));

        world.registerEntity(id);

        return debugComponent;
    }

    ECSE::World world;
    ECSE::CollisionSystem* system;
};

TEST_F(CollisionSystemTest, InvertCollisionTest)
{
    auto debugA = createCircle(sf::Vector2f(0.f, 0.f), sf::Vector2f(0.f, 10.f), 1.f);
    auto debugB = createCircle(sf::Vector2f(0.f, 10.f), sf::Vector2f(0.f, 10.f), 1.f);

    world.update(sf::Time::Zero);
    world.advance();

    auto collisionA = debugA->collisions[0];
    auto collisionB = debugB->collisions[0];

    ASSERT_EQ(collisionA.self, collisionB.other);
    ASSERT_EQ(collisionA.position, collisionB.otherPosition);
    ASSERT_EQ(collisionA.time, collisionB.time);
}

TEST_F(CollisionSystemTest, CollisionDataTest)
{
    auto debug = createCircle(sf::Vector2f(0.f, 0.f), sf::Vector2f(0.f, 10.f), 1.f);
    createCircle(sf::Vector2f(0.f, 10.f), sf::Vector2f(0.f, 10.f), 1.f);

    world.update(sf::Time::Zero);
    world.advance();

    auto collision = debug->collisions[0];

    auto entities = world.getEntities();
    ECSE::Entity* entA = entities[0];
    ECSE::Entity* entB = entities[1];

    ASSERT_EQ(entA, collision.self);
    ASSERT_EQ(entB, collision.other);
    ASSERT_FLOAT_EQ(0.f, collision.position.x);
    ASSERT_FLOAT_EQ(8.f, collision.position.y);
    ASSERT_FLOAT_EQ(0.f, collision.otherPosition.x);
    ASSERT_FLOAT_EQ(10.f, collision.otherPosition.y);
    ASSERT_FLOAT_EQ(0.8f, collision.time);
}

TEST_F(CollisionSystemTest, StartInsideStillCollisionTest)
{
    auto debugA = createCircle(sf::Vector2f(0.f, 0.f), sf::Vector2f(0.f, 0.f), 10.f);
    auto debugB = createCircle(sf::Vector2f(0.f, 10.f), sf::Vector2f(0.f, 10.f), 10.f);

    world.update(sf::Time::Zero);
    world.advance();

    ASSERT_EQ(1, debugA->collisions.size());
    ASSERT_EQ(1, debugB->collisions.size());
}

TEST_F(CollisionSystemTest, StartInsideMovingCollisionTest)
{
    auto debugA = createCircle(sf::Vector2f(0.f, 0.f), sf::Vector2f(0.f, 0.f), 10.f);
    auto debugB = createCircle(sf::Vector2f(0.f, 10.f), sf::Vector2f(0.f, -10.f), 10.f);

    world.update(sf::Time::Zero);
    world.advance();

    ASSERT_EQ(1, debugA->collisions.size());
    ASSERT_EQ(1, debugB->collisions.size());
}

TEST_F(CollisionSystemTest, MovingStillCollisionTest)
{
    auto debugA = createCircle(sf::Vector2f(0.f, 0.f), sf::Vector2f(50.f, 50.f), 5.f);
    auto debugB = createCircle(sf::Vector2f(30.f, 30.f), sf::Vector2f(30.f, 30.f), 4.f);

    world.update(sf::Time::Zero);
    world.advance();

    ASSERT_EQ(1, debugA->collisions.size());
    ASSERT_EQ(1, debugB->collisions.size());
}

TEST_F(CollisionSystemTest, StillMovingCollisionTest)
{
    auto debugA = createCircle(sf::Vector2f(30.f, 30.f), sf::Vector2f(30.f, 30.f), 4.f);
    auto debugB = createCircle(sf::Vector2f(0.f, 0.f), sf::Vector2f(50.f, 50.f), 5.f);

    world.update(sf::Time::Zero);
    world.advance();

    ASSERT_EQ(1, debugA->collisions.size());
    ASSERT_EQ(1, debugB->collisions.size());
}

TEST_F(CollisionSystemTest, MovingMovingCollisionTest)
{
    auto debugA = createCircle(sf::Vector2f(0.f, 10.f), sf::Vector2f(0.f, 50.f), 3.f);
    auto debugB = createCircle(sf::Vector2f(20.f, 30.f), sf::Vector2f(-20.f, 30.f), 3.f);

    world.update(sf::Time::Zero);
    world.advance();

    ASSERT_EQ(1, debugA->collisions.size());
    ASSERT_EQ(1, debugB->collisions.size());
}

TEST_F(CollisionSystemTest, NearMissTest)
{
    auto debugA = createCircle(sf::Vector2f(350.f, 10.f), sf::Vector2f(370.f, 75.f), 5.f);
    auto debugB = createCircle(sf::Vector2f(325.f, 25.f), sf::Vector2f(375.f, 25.f), 5.f);

    world.update(sf::Time::Zero);
    world.advance();

    ASSERT_EQ(0, debugA->collisions.size());
    ASSERT_EQ(0, debugB->collisions.size());
}

TEST_F(CollisionSystemTest, MultiCollisionTest)
{
    auto debugA = createCircle(sf::Vector2f(-100.f, 0.f), sf::Vector2f(100.f, 0.f), 10.f);
    auto debugB = createCircle(sf::Vector2f(0.f, 0.f), sf::Vector2f(0.f, 100.f), 5.f);
    auto debugC = createCircle(sf::Vector2f(0.f, -100.f), sf::Vector2f(0.f, 100.f), 3.f);

    world.update(sf::Time::Zero);
    world.advance();

    ASSERT_EQ(1, debugA->collisions.size());
    ASSERT_EQ(1, debugB->collisions.size());
    ASSERT_EQ(2, debugC->collisions.size());
}

TEST_F(CollisionSystemTest, MultiCollisionDisableTest)
{
    auto debugA = createCircle(sf::Vector2f(-100.f, 0.f), sf::Vector2f(100.f, 0.f), 10.f);
    auto debugB = createCircle(sf::Vector2f(0.f, 0.f), sf::Vector2f(0.f, 100.f), 5.f);
    auto debugC = createCircle(sf::Vector2f(0.f, -100.f), sf::Vector2f(0.f, 100.f), 3.f);
    debugC->disableOnHit = true;

    world.update(sf::Time::Zero);
    world.advance();

    ASSERT_EQ(1, debugA->collisions.size());
    ASSERT_EQ(0, debugB->collisions.size());
    ASSERT_EQ(1, debugC->collisions.size());
}

TEST_F(CollisionSystemTest, DiscreteJumpTest)
{
    auto debugA = createCircle(sf::Vector2f(0.f, 10.f), sf::Vector2f(0.f, 50.f), 3.f, true);
    auto debugB = createCircle(sf::Vector2f(20.f, 30.f), sf::Vector2f(-20.f, 30.f), 3.f);

    world.update(sf::Time::Zero);
    world.advance();

    ASSERT_EQ(0, debugA->collisions.size());
    ASSERT_EQ(0, debugB->collisions.size());
}

TEST_F(CollisionSystemTest, CircleLineHitTest)
{
    auto debugA = createCircle(sf::Vector2f(20.f, 0.f), sf::Vector2f(20.f, 40.f), 3.f);
    auto debugB = createLine(sf::Vector2f(0.f, 20.f), sf::Vector2f(0.f, 20.f), sf::Vector2f(100.f, 0.f));

    world.update(sf::Time::Zero);
    world.advance();

    ASSERT_EQ(1, debugA->collisions.size());
    ASSERT_EQ(1, debugB->collisions.size());
}

TEST_F(CollisionSystemTest, CircleLineMissTest)
{
    auto debugA = createCircle(sf::Vector2f(20.f, 0.f), sf::Vector2f(20.f, 40.f), 3.f);
    auto debugB = createLine(sf::Vector2f(40.f, 20.f), sf::Vector2f(80.f, 20.f), sf::Vector2f(10.f, 5.f));

    world.update(sf::Time::Zero);
    world.advance();

    ASSERT_EQ(0, debugA->collisions.size());
    ASSERT_EQ(0, debugB->collisions.size());
}

TEST_F(CollisionSystemTest, LineCircleHitTest)
{
    auto debugA = createLine(sf::Vector2f(0.f, 20.f), sf::Vector2f(40.f, 20.f), sf::Vector2f(10.f, 5.f));
    auto debugB = createCircle(sf::Vector2f(20.f, 0.f), sf::Vector2f(20.f, 40.f), 3.f);

    world.update(sf::Time::Zero);
    world.advance();

    ASSERT_EQ(1, debugA->collisions.size());
    ASSERT_EQ(1, debugB->collisions.size());
}

TEST_F(CollisionSystemTest, LineCircleMissTest)
{
    auto debugA = createLine(sf::Vector2f(0.f, 20.f), sf::Vector2f(40.f, 20.f), sf::Vector2f(10.f, 5.f));
    auto debugB = createCircle(sf::Vector2f(60.f, 0.f), sf::Vector2f(60.f, 40.f), 3.f);

    world.update(sf::Time::Zero);
    world.advance();

    ASSERT_EQ(0, debugA->collisions.size());
    ASSERT_EQ(0, debugB->collisions.size());
}

TEST_F(CollisionSystemTest, OffsetHitTest)
{
    auto debugA = createCircle(sf::Vector2f(-10.f, 0.f), sf::Vector2f(20.f, 0.f), 3.f, false, sf::Vector2f(0.f, 10.f));
    auto debugB = createCircle(sf::Vector2f(30.f, 10.f), sf::Vector2f(-10.f, 10.f), 3.f, false, sf::Vector2f(0.f, 0.f));

    world.update(sf::Time::Zero);
    world.advance();

    ASSERT_EQ(1, debugA->collisions.size());
    ASSERT_EQ(1, debugB->collisions.size());
}

TEST_F(CollisionSystemTest, OffsetMissTest)
{
    auto debugA = createCircle(sf::Vector2f(-10.f, 0.f), sf::Vector2f(20.f, 0.f), 3.f, false, sf::Vector2f(0.f, -10.f));
    auto debugB = createCircle(sf::Vector2f(30.f, 10.f), sf::Vector2f(-10.f, 10.f), 3.f, false, sf::Vector2f(0.f, 0.f));

    world.update(sf::Time::Zero);
    world.advance();

    ASSERT_EQ(0, debugA->collisions.size());
    ASSERT_EQ(0, debugB->collisions.size());
}