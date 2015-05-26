#include "gtest/gtest.h"
#include "ECSE/World.h"
#include "TestFixtures.h"
#include "TestUtils.h"

class WorldTest : public ::testing::Test
{
public:
    WorldTest()
        : world(nullptr)
    {
    }

    ECSE::World world;
};

class OrderedDummyWorldSystem : public DummyWorldSystem
{
public:
    explicit OrderedDummyWorldSystem(ECSE::World* world)
        : DummyWorldSystem(world)
    {
    }

    virtual std::string getID() const = 0;

    virtual void update(sf::Time deltaTime) override
    {
        DummyWorldSystem::update(deltaTime);

        if (updateOrder != nullptr)
        {
            updateOrder->push_back(getID());
        }
    }

    std::vector<std::string>* updateOrder = nullptr;
};

class WorldTest_SysA;
class WorldTest_SysB;
class WorldTest_SysC;

class WorldTest_SysA : public OrderedDummyWorldSystem
{
public:
    explicit WorldTest_SysA(ECSE::World* world)
        : OrderedDummyWorldSystem(world)
    {
    }

    std::string getID() const override
    {
        return "WorldTest_SysA";
    }

    void added() override
    {
        if (world->getSystem<WorldTest_SysB>() == nullptr)
        {
            throw std::runtime_error("WorldTest_SysA requires WorldTest_SysB");
        }

        OrderedDummyWorldSystem::added();
    }
};

class WorldTest_SysB : public OrderedDummyWorldSystem
{
public:
    explicit WorldTest_SysB(ECSE::World* world)
        : OrderedDummyWorldSystem(world)
    {
    }

    std::string getID() const override
    {
        return "WorldTest_SysB";
    }
};

class WorldTest_SysC : public OrderedDummyWorldSystem
{
public:
    explicit WorldTest_SysC(ECSE::World* world)
        : OrderedDummyWorldSystem(world)
    {
    }

    std::string getID() const override
    {
        return "WorldTest_SysC";
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

        OrderedDummyWorldSystem::added();
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

TEST_F(WorldTest, TestGetNullSystem)
{
    ASSERT_EQ(nullptr, world.getSystem<WorldTest_SysA>());
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

TEST_F(WorldTest, TestUpdateOrder)
{
    std::vector<std::string> updateOrder;

    world.addSystem<WorldTest_SysA>()->updateOrder = &updateOrder;
    world.addSystem<WorldTest_SysC>()->updateOrder = &updateOrder;
    world.addSystem<WorldTest_SysB>()->updateOrder = &updateOrder;

    world.update(sf::Time::Zero);

    ASSERT_EQ(3, updateOrder.size());
    ASSERT_EQ("WorldTest_SysA", updateOrder[0]);
    ASSERT_EQ("WorldTest_SysC", updateOrder[1]);
    ASSERT_EQ("WorldTest_SysB", updateOrder[2]);
}

TEST_F(WorldTest, TestRegisterEntity)
{
    DummyWorldSystem* sys = world.addSystem<DummyWorldSystem>();
    sys->passChecks = true;

    ECSE::Entity::ID id = world.createEntity();
    ECSE::Entity* e = world.registerEntity(id);

    // Allow system to update internal list
    world.update(sf::Time::Zero);
    world.advance();

    auto entities = sys->getEntities();

    ASSERT_EQ(1, entities.size());
    ASSERT_TRUE(contains(entities, e));
}

TEST_F(WorldTest, TestAttachComponent)
{
    ECSE::Entity::ID id = world.createEntity();
    world.attachComponent<DummyComponent>(id);
    ECSE::Entity* e = world.registerEntity(id);

    ASSERT_NE(nullptr, e->getComponent<DummyComponent>());
}

TEST_F(WorldTest, TestAttachComponentAfterRegister)
{
    ECSE::Entity::ID id = world.createEntity();
    world.registerEntity(id);

    ASSERT_THROW(
        world.attachComponent<DummyComponent>(id),
        std::runtime_error);
}

TEST_F(WorldTest, TestAttachComponentInvalidID)
{
    ASSERT_THROW(
        world.attachComponent<DummyComponent>(ECSE::Entity::invalidID),
        std::runtime_error);
}

TEST_F(WorldTest, TestDestroyEntity)
{
    DummyWorldSystem* sys = world.addSystem<DummyWorldSystem>();
    sys->passChecks = true;

    ECSE::Entity::ID id = world.createEntity();
    world.registerEntity(id);

    // Allow system to update internal list
    world.update(sf::Time::Zero);
    world.advance();

    world.destroyEntity(id);

    world.update(sf::Time::Zero);
    world.advance();

    ASSERT_EQ(0, world.getEntities().size());
    ASSERT_EQ(0, sys->getEntities().size());
    ASSERT_EQ(nullptr, world.getEntity(id));
}