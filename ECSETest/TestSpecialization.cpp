#include "gtest/gtest.h"
#include "ECSE/World.h"
#include "ECSE/SpecializationSystem.h"
#include "ECSE/SpecializationComponent.h"

class DummySpecialization : public ECSE::Specialization
{
public:
    void init() override
    {
        initialized = true;
    }

    void update(sf::Time) override
    {
        updated = true;
    }

    void advance() override
    {
        advanced = true;
    }

    void render(float, sf::RenderTarget&) override
    {
        rendered = true;
    }

    bool initialized = false;
    bool updated = false;
    bool advanced = false;
    bool rendered = false;
};

class SpecializationComponentTest : public ::testing::Test
{
public:
    explicit SpecializationComponentTest()
        : world(nullptr)
    {
        world.addSystem<ECSE::SpecializationSystem>();

        entId = world.createEntity();
        component = world.attachComponent<ECSE::SpecializationComponent>(entId);
        world.registerEntity(entId);
    }

    ECSE::Entity::ID entId;
    ECSE::World world;
    ECSE::SpecializationComponent* component;
};

class SpecializationTest : public SpecializationComponentTest
{
public:
    explicit SpecializationTest()
        : SpecializationComponentTest()
    {
        component->setSpecialization(std::make_unique<DummySpecialization>());
        spec = dynamic_cast<DummySpecialization*>(component->getSpecialization());
    }

    DummySpecialization* spec;
};

TEST_F(SpecializationComponentTest, SetSpecializationTest)
{
    component->setSpecialization(std::make_unique<DummySpecialization>());

    ASSERT_NE(nullptr, component->getSpecialization());
}

TEST_F(SpecializationComponentTest, SetSpecializationNullTest)
{
    ASSERT_THROW(component->setSpecialization(nullptr), std::runtime_error);
}

TEST_F(SpecializationComponentTest, SetSpecializationTwiceTest)
{
    component->setSpecialization(std::make_unique<DummySpecialization>());

    ASSERT_THROW(component->setSpecialization(std::make_unique<DummySpecialization>()), std::runtime_error);
}

TEST_F(SpecializationTest, AdvanceTest)
{
    // Need to update first to add it to the system
    world.update(sf::seconds(0.1f));
    world.advance();

    ASSERT_TRUE(spec->initialized);
    ASSERT_TRUE(spec->advanced);
}

TEST_F(SpecializationTest, UpdateTest)
{
    world.update(sf::seconds(0.1f));

    ASSERT_TRUE(spec->updated);
}

TEST_F(SpecializationTest, RenderTest)
{
    // Need to update first to add it to the system
    world.update(sf::seconds(0.1f));

    sf::RenderTexture target;
    world.render(0.f, target);

    ASSERT_TRUE(spec->rendered);
}