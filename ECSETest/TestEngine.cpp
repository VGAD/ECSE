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
    
    virtual void activate() override { ++activateCount; }
    virtual void deactivate() override { ++deactivateCount; }
    virtual void update(sf::Time) override { ++updateCount; }
    virtual void advance() override { ++advanceCount; }
    virtual void render(float, sf::RenderTarget&) override {
        ++renderCount;
        
        // Let at least one advance occur
        if (engine->getTicks() > 0)
        {
            engine->exit();
        }
    }

    virtual std::string getName() override { return "DummyState"; }

    size_t activateCount = 0;
    size_t deactivateCount = 0;
    size_t advanceCount = 0;
    size_t updateCount = 0;
    size_t renderCount = 0;
};

class EngineTest : public ::testing::Test
{
protected:
    EngineTest() : engine(sf::Vector2i(800, 600)) {}

    virtual void SetUp() override
    {
        state = engine.pushState<DummyState>();

        ASSERT_FALSE(state == nullptr) << "State should be returned";

        engine.run();
    }

    ECSE::Engine engine;
    DummyState* state;
};

TEST_F(EngineTest, ActivateTest)
{
    ASSERT_EQ(1, state->activateCount) << "Activation should only occur once";
}

TEST_F(EngineTest, DeactivateTest)
{
    ASSERT_EQ(0, state->deactivateCount) << "State is still on top, so should not be deactivated";
}

TEST_F(EngineTest, UpdateAdvanceTest)
{
    ASSERT_GT(state->updateCount, size_t(0)) << "State should update at least once";
    ASSERT_GT(state->advanceCount, size_t(0)) << "State should advance at least once";
    ASSERT_EQ(state->updateCount - 1, state->advanceCount) << "State should have advanced one less time than updated";
}

TEST_F(EngineTest, TickTest)
{
    ASSERT_EQ(state->advanceCount, engine.getTicks()) << "Tick count should equal advance count";
}

TEST_F(EngineTest, RenderTest)
{
    ASSERT_GT(state->renderCount, size_t(0)) << "State should have rendered at least one frame";
}

TEST_F(EngineTest, FrameTest)
{
    ASSERT_EQ(state->renderCount, engine.getFrames()) << "Frame count should equal render count";
}