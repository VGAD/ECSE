#include "gtest/gtest.h"
#include "ECSE/EntityManager.h"
#include "TestUtils.h"

class EntityManagerTest : public ::testing::Test
{
public:
    ECSE::EntityManager manager;
};

TEST_F(EntityManagerTest, EmptyTest)
{
    ASSERT_TRUE(manager.getEntities().empty()) << "No entities should be returned";
}

TEST_F(EntityManagerTest, CreateEntityTest)
{
    ECSE::Entity::ID eID = manager.createEntity();
    ASSERT_NE(ECSE::Entity::invalidID, eID) << "Entity should have a valid ID";
}

TEST_F(EntityManagerTest, GetEntityTest)
{
    ECSE::Entity::ID eID = manager.createEntity();

    ASSERT_NE(nullptr, manager.getEntity(eID)) << "Entity should have a valid pointer";
}

TEST_F(EntityManagerTest, DestroyEntityTest)
{
    manager.createEntity();
    ECSE::Entity::ID eID = manager.createEntity();
    manager.createEntity();

    manager.destroyEntity(eID);

    ASSERT_EQ(2, manager.getEntities().size()) << "One entity should have been removed";
}

TEST_F(EntityManagerTest, DestroyEntityByPointerTest)
{
    manager.createEntity();
    ECSE::Entity::ID eID = manager.createEntity();
    manager.createEntity();

    ECSE::Entity* ptr = manager.getEntity(eID);
    manager.destroyEntity(ptr);

    ASSERT_EQ(2, manager.getEntities().size()) << "One entity should have been removed";
}

TEST_F(EntityManagerTest, GetEntitiesTest)
{
    std::vector<ECSE::Entity*> added;
    for (size_t i = 0; i < 5; ++i)
    {
        ECSE::Entity::ID eID = manager.createEntity();
        added.push_back(manager.getEntity(eID));
    }

    auto entities = manager.getEntities();
    for (ECSE::Entity* e : added)
    {
        ASSERT_TRUE(contains(entities, e)) << "Entity should have been in vector";
    }

    for (ECSE::Entity* e : entities)
    {
        ASSERT_TRUE(contains(entities, e)) << "Entity was not added, but is in vector anyway";
    }
}

TEST_F(EntityManagerTest, NewIDTest)
{
    manager.createEntity();
    ECSE::Entity::ID eID = manager.createEntity();
    manager.createEntity();

    manager.destroyEntity(eID);

    ECSE::Entity::ID newID = manager.createEntity();

    ASSERT_NE(eID, newID) << "Entity limit not exceeded, so fresh IDs should be used";
}

TEST_F(EntityManagerTest, ReplaceMemoryTest)
{
    manager.createEntity();
    ECSE::Entity::ID eID = manager.createEntity();
    manager.createEntity();

    ECSE::Entity* entityA = manager.getEntity(eID);

    manager.destroyEntity(eID);

    ECSE::Entity::ID newID = manager.createEntity();
    ECSE::Entity* entityB = manager.getEntity(newID);

    ASSERT_EQ(entityA, entityB) << "Entities should backfill unused memory";
}

class SmallMaxIDEntityManager : public ECSE::EntityManager
{
public:
    ECSE::Entity::ID getMaxIDCount() const override
    {
        return 4;
    }
};

class SmallMaxIDEntityManagerTest : public ::testing::Test
{
public:
    SmallMaxIDEntityManager manager;
};

TEST_F(SmallMaxIDEntityManagerTest, TooManyIDsTest)
{
    manager.createEntity();
    manager.createEntity();
    manager.createEntity();

    ASSERT_THROW(manager.createEntity(), std::runtime_error) << "Should complain about too many IDs";
}

TEST_F(SmallMaxIDEntityManagerTest, ReuseIDTest)
{
    manager.createEntity();
    ECSE::Entity::ID eID = manager.createEntity();
    manager.createEntity();

    manager.destroyEntity(eID);

    ECSE::Entity::ID newID = manager.createEntity();

    ASSERT_EQ(eID, newID) << "ID should be reused";
}
