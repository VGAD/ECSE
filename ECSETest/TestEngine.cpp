#include "gtest/gtest.h"
#include "ECSE/Engine.h"
#include "ECSE/Logging.h"
#include "SFML/System.hpp"

class DummyState : public ECSE::State
{
public:
    explicit DummyState(ECSE::Engine* engine)
        : ECSE::State(engine)
    {
    }
    
    virtual void activate() override { }
    virtual void deactivate() override { }
    virtual void update(sf::Time) override { }
    virtual void advance() override {}
    virtual void render(float) override {}

    virtual std::string getName() override { return "DummyState"; }
};

class EngineTest : public ::testing::Test
{
protected:
    virtual void SetUp() override
    {
        ECSE::Engine engine(sf::Vector2i(800, 600));
        engine.pushState<DummyState>();
    }
};

TEST_F(EngineTest, TestStart)
{
    
}