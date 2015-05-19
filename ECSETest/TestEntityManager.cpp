#include "TestFixtures.h"
#include "ECSE/EntityManager.h"

class EntityManagerTest : public ::testing::Test
{
public:
    ECSE::EntityManager manager;
};

TEST_F(EntityManagerTest, TestEmpty)
{
    ASSERT_TRUE(manager.getEntities().empty()) << "No entities should be returned";
}

TEST_F(EntityManagerTest, TestCreateEntity)
{
    ECSE::Entity::ID eID = manager.createEntity();
    ASSERT_NE(ECSE::Entity::invalidID, eID) << "Entity should have a valid ID";
}

TEST_F(EntityManagerTest, TestGetEntity)
{
    ECSE::Entity::ID eID = manager.createEntity();

    ASSERT_NE(nullptr, manager.getEntity(eID)) << "Entity should have a valid pointer";
}

TEST_F(EntityManagerTest, TestDestroyEntity)
{
    manager.createEntity();
    ECSE::Entity::ID eID = manager.createEntity();
    manager.createEntity();

    manager.destroyEntity(eID);

    ASSERT_EQ(2, manager.getEntities().size()) << "One entity should have been removed";
}

TEST_F(EntityManagerTest, TestDestroyEntityByPointer)
{
    manager.createEntity();
    ECSE::Entity::ID eID = manager.createEntity();
    manager.createEntity();

    ECSE::Entity* ptr = manager.getEntity(eID);
    manager.destroyEntity(ptr);

    ASSERT_EQ(2, manager.getEntities().size()) << "One entity should have been removed";
}

TEST_F(EntityManagerTest, TestGetEntities)
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
        ASSERT_TRUE(std::find(entities.begin(), entities.end(), e) != entities.end()) << "Entity should have been in vector";
    }

    for (ECSE::Entity* e : entities)
    {
        ASSERT_TRUE(std::find(added.begin(), added.end(), e) != added.end()) << "Entity was not added, but is in vector anyway";
    }
}

TEST_F(EntityManagerTest, TestNewID)
{
    manager.createEntity();
    ECSE::Entity::ID eID = manager.createEntity();
    manager.createEntity();

    manager.destroyEntity(eID);

    ECSE::Entity::ID newID = manager.createEntity();

    ASSERT_NE(eID, newID) << "Entity limit not exceeded, so fresh IDs should be used";
}

TEST_F(EntityManagerTest, TestReplaceMemory)
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

TEST_F(SmallMaxIDEntityManagerTest, TestTooManyIDs)
{
    manager.createEntity();
    manager.createEntity();
    manager.createEntity();

    ASSERT_THROW(manager.createEntity(), std::runtime_error) << "Should complain about too many IDs";
}

TEST_F(SmallMaxIDEntityManagerTest, TestReuseID)
{
    manager.createEntity();
    ECSE::Entity::ID eID = manager.createEntity();
    manager.createEntity();

    manager.destroyEntity(eID);

    ECSE::Entity::ID newID = manager.createEntity();

    ASSERT_EQ(eID, newID) << "ID should be reused";
}