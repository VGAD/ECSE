#include "gtest/gtest.h"
#include "ECSE/World.h"
#include "TestFixtures.h"

class WorldTest : public ::testing::Test
{
public:
    WorldTest()
        : world(nullptr)
    {
    }

    ECSE::World world;
};

class WorldTest_SysA;
class WorldTest_SysB;
class WorldTest_SysC;

class WorldTest_SysA : public DummyWorldSystem
{
public:
    explicit WorldTest_SysA(ECSE::World* world)
        : DummyWorldSystem(world)
    {
    }

    void added() override
    {
        if (world->getSystem<WorldTest_SysB>() == nullptr)
        {
            throw std::runtime_error("WorldTest_SysA requires WorldTest_SysB");
        }

        DummyWorldSystem::added();
    }
};

class WorldTest_SysB : public DummyWorldSystem
{
public:
    explicit WorldTest_SysB(ECSE::World* world)
        : DummyWorldSystem(world)
    {
    }
};

class WorldTest_SysC : public DummyWorldSystem
{
public:
    explicit WorldTest_SysC(ECSE::World* world)
        : DummyWorldSystem(world)
    {
    }

    void added() override
    {
        if (world->getSystem<WorldTest_SysA>() == nullptr)
        {
            throw std::runtime_error("WorldTest_SysC requires WorldTest_SysA");
        }

        if (world->getSystem<WorldTest_SysB>() == nullptr)
        {
            throw std::runtime_error("WorldTest_SysC requires WorldTest_SysB");
        }

        DummyWorldSystem::added();
    }
};

TEST_F(WorldTest, TestAddSystems)
{
    world.addSystem<WorldTest_SysA>();
    world.addSystem<WorldTest_SysB>();
    world.addSystem<WorldTest_SysC>();

    ASSERT_NE(nullptr, world.getSystem<WorldTest_SysA>());
    ASSERT_NE(nullptr, world.getSystem<WorldTest_SysB>());
    ASSERT_NE(nullptr, world.getSystem<WorldTest_SysC>());
}

TEST_F(WorldTest, TestAddSameSystem)
{
    world.addSystem<WorldTest_SysA>();

    ASSERT_THROW(
        world.addSystem<WorldTest_SysA>(),
        std::runtime_error);
}

TEST_F(WorldTest, TestAddSystemAfterUpdate)
{
    world.update(sf::Time::Zero);

    ASSERT_THROW(
        world.addSystem<DummyWorldSystem>(),
        std::runtime_error);
}

TEST_F(WorldTest, TestPassSystemPrereqs)
{
    world.addSystem<WorldTest_SysA>();
    world.addSystem<WorldTest_SysB>();
    world.addSystem<WorldTest_SysC>();

    world.update(sf::Time::Zero);

    ASSERT_TRUE(world.getSystem<WorldTest_SysA>()->isAdded);
    ASSERT_TRUE(world.getSystem<WorldTest_SysB>()->isAdded);
    ASSERT_TRUE(world.getSystem<WorldTest_SysC>()->isAdded);
}

TEST_F(WorldTest, TestFailSystemPrereqs)
{
    world.addSystem<WorldTest_SysB>();
    world.addSystem<WorldTest_SysC>();

    ASSERT_THROW(
        world.update(sf::Time::Zero),
        std::runtime_error);

    ASSERT_TRUE(world.getSystem<WorldTest_SysB>()->isAdded);
}

//TODO: still need to test update order, null getSystem(), component stuff, entity register/destroy