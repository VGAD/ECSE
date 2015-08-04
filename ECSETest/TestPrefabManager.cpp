#include "gtest/gtest.h"
#include "ECSE/World.h"
#include "ECSE/PrefabManager.h"

class PrefabTest : public ::testing::Test
{
public:
    explicit PrefabTest()
        : world(nullptr)
    {
    }

    ECSE::PrefabManager manager;
    ECSE::World world;
};

class ComponentA : public ECSE::Component {};
class ComponentB : public ECSE::Component {};
class ComponentC : public ECSE::Component {};

TEST_F(PrefabTest, AddPrefabTest)
{
    manager.addPrefab("test", ECSE::PrefabManager::Prefab{});
}

TEST_F(PrefabTest, ApplyPrefabTest)
{
    manager.addPrefab("AC", [](ECSE::World& world, ECSE::Entity::ID entId)
    {
        world.attachComponent<ComponentA>(entId);
        world.attachComponent<ComponentC>(entId);
    });

    auto testEntId = world.createEntity();
    manager.applyPrefab("AC", world, testEntId);

    auto testEnt = world.getEntity(testEntId);

    ASSERT_NE(nullptr, testEnt->getComponent<ComponentA>());
    ASSERT_EQ(nullptr, testEnt->getComponent<ComponentB>());
    ASSERT_NE(nullptr, testEnt->getComponent<ComponentC>());
}

TEST_F(PrefabTest, CreateEntityTest)
{
    manager.addPrefab("BC", [](ECSE::World& world, ECSE::Entity::ID entId)
    {
        world.attachComponent<ComponentB>(entId);
        world.attachComponent<ComponentC>(entId);
    });

    auto testEntId = manager.createEntity("BC", world);

    auto testEnt = world.getEntity(testEntId);

    ASSERT_EQ(nullptr, testEnt->getComponent<ComponentA>());
    ASSERT_NE(nullptr, testEnt->getComponent<ComponentB>());
    ASSERT_NE(nullptr, testEnt->getComponent<ComponentC>());
}