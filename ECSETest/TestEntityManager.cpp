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