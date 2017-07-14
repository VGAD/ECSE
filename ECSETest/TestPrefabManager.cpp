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
    manager.addPrefab("AC", [](ECSE::World& world, ECSE::Entity& entity, ECSE::PrefabManager::Properties)
    {
        world.attachComponent<ComponentA>(entity);
        world.attachComponent<ComponentC>(entity);
    });

    auto testEntId = world.createEntity();
    auto testEnt = world.getEntity(testEntId);

    manager.applyPrefab("AC", world, *testEnt);

    ASSERT_NE(nullptr, testEnt->getComponent<ComponentA>());
    ASSERT_EQ(nullptr, testEnt->getComponent<ComponentB>());
    ASSERT_NE(nullptr, testEnt->getComponent<ComponentC>());
}

TEST_F(PrefabTest, CreateEntityTest)
{
    manager.addPrefab("BC", [](ECSE::World& world, ECSE::Entity& entity, ECSE::PrefabManager::Properties)
    {
        world.attachComponent<ComponentB>(entity);
        world.attachComponent<ComponentC>(entity);
    });

    auto testEntId = manager.createEntity("BC", world);
    auto testEnt = world.getEntity(testEntId);

    ASSERT_EQ(nullptr, testEnt->getComponent<ComponentA>());
    ASSERT_NE(nullptr, testEnt->getComponent<ComponentB>());
    ASSERT_NE(nullptr, testEnt->getComponent<ComponentC>());
}

TEST_F(PrefabTest, CreateEntityWithPropertiesTest)
{
    manager.addPrefab("props", [](ECSE::World& world, ECSE::Entity& entity, ECSE::PrefabManager::Properties props)
    {
        if (props["A"] == "true") world.attachComponent<ComponentA>(entity);
        if (props["B"] == "true") world.attachComponent<ComponentB>(entity);
        if (props["C"] == "true") world.attachComponent<ComponentC>(entity);
    });

    auto testEntId = manager.createEntity("props", world, {
        { "A", "true" },
        { "B", "true" },
        { "C", "false" }
    });

    auto testEnt = world.getEntity(testEntId);

    ASSERT_NE(nullptr, testEnt->getComponent<ComponentA>());
    ASSERT_NE(nullptr, testEnt->getComponent<ComponentB>());
    ASSERT_EQ(nullptr, testEnt->getComponent<ComponentC>());
}
