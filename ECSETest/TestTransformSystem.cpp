#include "gtest/gtest.h"
#include "TestUtils.h"
#include "ECSE/TransformSystem.h"
#include "ECSE/World.h"

class TransformSystemTest : public ::testing::Test
{
public:
    TransformSystemTest()
        : world(nullptr)
    {
    }

    void SetUp() override
    {
        system = world.addSystem<ECSE::TransformSystem>();

        a = createEntity();
        b = createEntity();

        transA = a->getComponent<ECSE::TransformComponent>();
        transB = b->getComponent<ECSE::TransformComponent>();
    }

    const ECSE::Entity* createEntity()
    {
        auto entId = world.createEntity();
        world.attachComponent<ECSE::TransformComponent>(entId);

        return world.getEntity(entId);
    }

    ECSE::World world;
    ECSE::TransformSystem* system;
    const ECSE::Entity* a, * b;
    ECSE::TransformComponent* transA, * transB;
};

TEST_F(TransformSystemTest, GlobalPositionTest)
{
    transA->setNextLocalPosition(sf::Vector2f(200.f, 100.f));
    transA->advance();

    ASSERT_EQ(transA->getLocalPosition(), system->getGlobalPosition(*a));
}

TEST_F(TransformSystemTest, ChildConvertLocalPositionTest)
{
    auto globalPos = sf::Vector2f{ 10.f, 5.f };

    transA->setNextLocalPosition(globalPos);
    transA->advance();

    transB->setNextLocalPosition(sf::Vector2f(8.f, 2.f));
    transB->advance();

    system->parentEntity(*a, *b);

    auto localPos = transA->getLocalPosition();
    auto newGlobalPos = system->getGlobalPosition(*a);

    ASSERT_NEAR_TRANS(globalPos.x, newGlobalPos.x);
    ASSERT_NEAR_TRANS(globalPos.y, newGlobalPos.y);

    ASSERT_NEAR_TRANS(2.f, localPos.x);
    ASSERT_NEAR_TRANS(3.f, localPos.y);
}

TEST_F(TransformSystemTest, ChildConvertLocalAngleTest)
{
    auto globalAngle = 25.f;

    transA->setNextLocalAngle(globalAngle);
    transA->advance();

    transB->setNextLocalAngle(10.f);
    transB->advance();

    system->parentEntity(*a, *b);

    ASSERT_NEAR_TRANS(globalAngle, system->getGlobalAngle(*a));
    ASSERT_NEAR_TRANS(15.f, transA->getLocalAngle());
}

TEST_F(TransformSystemTest, ChildConvertLocalAnglePositionTest)
{
    auto globalPos = sf::Vector2f{ 5.f, 1.f };
    auto globalAngle = ECSE::quarterPi;

    transA->setNextLocalPosition(globalPos);
    transA->setNextLocalAngle(globalAngle);
    transA->advance();

    transB->setNextLocalPosition(sf::Vector2f(1.f, 2.f));
    transB->setNextLocalAngle(ECSE::halfPi);
    transB->advance();

    system->parentEntity(*a, *b);

    auto localPos = transA->getLocalPosition();
    auto newGlobalPos = system->getGlobalPosition(*a);

    ASSERT_NEAR_TRANS(globalPos.x, newGlobalPos.x);
    ASSERT_NEAR_TRANS(globalPos.y, newGlobalPos.y);
    ASSERT_NEAR_TRANS(globalAngle, system->getGlobalAngle(*a));

    ASSERT_NEAR_TRANS(-1.f, localPos.x);
    ASSERT_NEAR_TRANS(-4.f, localPos.y);
    ASSERT_NEAR_TRANS(-ECSE::quarterPi, transA->getLocalAngle());
}

TEST_F(TransformSystemTest, ChildRevertLocalPositionTest)
{
    auto globalPos = sf::Vector2f(2.f, 1.f);
    transA->setNextLocalPosition(globalPos);
    transA->advance();

    transB->setNextLocalPosition(sf::Vector2f(-12.f, 7.f));
    transB->advance();

    system->parentEntity(*a, *b);
    system->unparentEntity(*a);

    auto localPos = transA->getLocalPosition();
    auto newGlobalPos = system->getGlobalPosition(*a);

    ASSERT_NEAR_TRANS(globalPos.x, newGlobalPos.x);
    ASSERT_NEAR_TRANS(globalPos.y, newGlobalPos.y);

    ASSERT_NEAR_TRANS(localPos.x, newGlobalPos.x);
    ASSERT_NEAR_TRANS(localPos.y, newGlobalPos.y);
}

TEST_F(TransformSystemTest, ChildRevertLocalAngleTest)
{
    auto globalAngle = 90.f;

    transA->setNextLocalAngle(globalAngle);
    transA->advance();

    transB->setNextLocalAngle(180.f);
    transB->advance();

    system->parentEntity(*a, *b);
    system->unparentEntity(*a);

    ASSERT_NEAR_TRANS(globalAngle, system->getGlobalAngle(*a));
    ASSERT_NEAR_TRANS(globalAngle, transA->getLocalAngle());
}

TEST_F(TransformSystemTest, ChildRevertLocalAnglePositionTest)
{
    auto globalPos = sf::Vector2f{ 8.f, -4.f };
    auto globalAngle = 11.f;

    transA->setNextLocalPosition(globalPos);
    transA->setNextLocalAngle(globalAngle);
    transA->advance();

    transB->setNextLocalPosition(sf::Vector2f(4.f, 5.f));
    transB->setNextLocalAngle(-23.f);
    transB->advance();

    system->parentEntity(*a, *b);
    system->unparentEntity(*a);

    auto localPos = transA->getLocalPosition();
    auto newGlobalPos = system->getGlobalPosition(*a);

    ASSERT_NEAR_TRANS(globalPos.x, newGlobalPos.x);
    ASSERT_NEAR_TRANS(globalPos.y, newGlobalPos.y);
    ASSERT_NEAR_TRANS(globalAngle, system->getGlobalAngle(*a));

    ASSERT_NEAR_TRANS(globalPos.x, localPos.x);
    ASSERT_NEAR_TRANS(globalPos.y, localPos.y);
    ASSERT_NEAR_TRANS(globalAngle, transA->getLocalAngle());
}

TEST_F(TransformSystemTest, ChildConvertLocalAngleNextPositionTest)
{
    auto globalPos = sf::Vector2f{ 5.f, 1.f };
    auto globalAngle = ECSE::quarterPi;

    transA->setNextLocalPosition(globalPos);
    transA->setNextLocalAngle(globalAngle);

    transB->setNextLocalPosition(sf::Vector2f(1.f, 2.f));
    transB->setNextLocalAngle(ECSE::halfPi);

    system->parentEntity(*a, *b);

    auto localPos = transA->getNextLocalPosition();
    auto newGlobalPos = system->getNextGlobalPosition(*a);

    ASSERT_NEAR_TRANS(globalPos.x, newGlobalPos.x);
    ASSERT_NEAR_TRANS(globalPos.y, newGlobalPos.y);
    ASSERT_NEAR_TRANS(globalAngle, system->getNextGlobalAngle(*a));

    ASSERT_NEAR_TRANS(-1.f, localPos.x);
    ASSERT_NEAR_TRANS(-4.f, localPos.y);
    ASSERT_NEAR_TRANS(-ECSE::quarterPi, transA->getNextLocalAngle());
}

TEST_F(TransformSystemTest, ChildConvertLocalAngleInterpPositionTest)
{
    auto globalPos = sf::Vector2f(0.f, 1.f);
    auto globalAngle = -ECSE::halfPi;

    transA->setNextLocalPosition(globalPos);
    transA->setNextLocalAngle(globalAngle);

    transB->setNextLocalPosition(sf::Vector2f(1.f, 0.f));
    transB->setNextLocalAngle(ECSE::halfPi);

    system->parentEntity(*a, *b);

    float delta = 0.5f;
    auto localPos = transA->getInterpLocalPosition(delta);
    auto newGlobalPos = system->getInterpGlobalPosition(*a, delta);

    ASSERT_NEAR_TRANS(0.5, newGlobalPos.x);
    ASSERT_NEAR_TRANS(0.70710678, newGlobalPos.y);
    ASSERT_NEAR_TRANS(-0.7853975f, system->getInterpGlobalAngle(*a, delta));

    ASSERT_NEAR_TRANS(0.5f, localPos.x);
    ASSERT_NEAR_TRANS(0.5f, localPos.y);
    ASSERT_NEAR_TRANS(-ECSE::halfPi, transA->getInterpLocalAngle(delta));
}