#include "gtest/gtest.h"
#include "ECSE/SetSystem.h"
#include "TestUtils.h"

// Test SetSystem because System is lacking features for a reasonable test
class DummySystem : public ECSE::SetSystem
{
public:
    explicit DummySystem()
        : SetSystem(nullptr)
    { }

    bool passChecks = true;

protected:
    bool checkRequirements(const ECSE::Entity&) const override
    {
        return passChecks;
    }
};

class SystemTest : public ::testing::Test
{
public:
    DummySystem system;
};

TEST_F(SystemTest, TestInspect)
{
    ECSE::Entity e;

    system.inspectEntity(e);
    system.advance();

    auto entities = system.getEntities();

    ASSERT_EQ(1, entities.size()) << "One entity should be added";
    ASSERT_TRUE(entities.find(&e) != entities.end()) << "Same entity should be added";
}

TEST_F(SystemTest, TestInspectTwice)
{
    ECSE::Entity e;

    system.inspectEntity(e);
    system.inspectEntity(e);
    system.advance();

    auto entities = system.getEntities();

    ASSERT_EQ(1, entities.size()) << "Entity should only be added once";
}

TEST_F(SystemTest, TestAddTwice)
{
    ECSE::Entity e;

    system.inspectEntity(e);
    system.advance();

    system.inspectEntity(e);
    system.advance();

    auto entities = system.getEntities();

    ASSERT_EQ(1, entities.size()) << "Entity should only be added once";
}

TEST_F(SystemTest, TestAddMultiple)
{
    ECSE::Entity added[5];

    for (auto& e : added)
    {
        system.inspectEntity(e);
    }
    system.advance();

    auto entities = system.getEntities();
    for (ECSE::Entity &e : added)
    {
        ASSERT_TRUE(contains(entities, &e)) << "Entity should have been added";
    }
}

TEST_F(SystemTest, TestRemove)
{
    ECSE::Entity e;

    system.inspectEntity(e);
    system.advance();

    system.markToRemove(e);
    system.advance();

    auto entities = system.getEntities();

    ASSERT_EQ(0, entities.size()) << "Entity should be removed";
}

TEST_F(SystemTest, TestMarkToRemoveTwice)
{
    ECSE::Entity e1;
    ECSE::Entity e2;

    system.inspectEntity(e1);
    system.inspectEntity(e2);
    system.advance();

    system.markToRemove(e1);
    system.markToRemove(e1);
    system.advance();

    auto entities = system.getEntities();

    ASSERT_EQ(1, entities.size()) << "Only one entity should be removed";
    ASSERT_EQ(&e2, *entities.begin()) << "Only first entity should be removed";
}

TEST_F(SystemTest, TestRemoveTwice)
{
    ECSE::Entity e1;
    ECSE::Entity e2;

    system.inspectEntity(e1);
    system.inspectEntity(e2);
    system.advance();

    system.markToRemove(e1);
    system.advance();

    system.markToRemove(e1);
    system.advance();

    auto entities = system.getEntities();

    ASSERT_EQ(1, entities.size()) << "Only one entity should be removed";
    ASSERT_EQ(&e2, *entities.begin()) << "Only first entity should be removed";
}

TEST_F(SystemTest, TestRemoveMultiple)
{
    ECSE::Entity added[5];

    for (auto& e : added)
    {
        system.inspectEntity(e);
    }
    system.advance();

    system.markToRemove(added[1]);
    system.markToRemove(added[4]);
    system.advance();

    auto entities = system.getEntities();
    ASSERT_TRUE(contains(entities, &added[0])) << "Entity should not have been removed";
    ASSERT_FALSE(contains(entities, &added[1])) << "Entity should have been removed";
    ASSERT_TRUE(contains(entities, &added[2])) << "Entity should not have been removed";
    ASSERT_TRUE(contains(entities, &added[3])) << "Entity should not have been removed";
    ASSERT_FALSE(contains(entities, &added[4])) << "Entity should have been removed";
}