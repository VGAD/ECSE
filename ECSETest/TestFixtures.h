#pragma once

#include "gtest/gtest.h"
#include "SFML/System.hpp"
#include "ECSE/Engine.h"
#include "ECSE/SetSystem.h"

//! A dummy state which does nothing other than to track each call.
class DummyState : public ECSE::State
{
public:
    explicit DummyState(ECSE::Engine* engine)
        : ECSE::State(engine)
    {
    }

    virtual void activate() override { ++activateCount; }
    virtual void deactivate() override { ++deactivateCount; }
    virtual void update(sf::Time time) override { State::update(time); ++updateCount; }
    virtual void render(float, sf::RenderTarget&) override { ++renderCount; }
    virtual void advance() override { ++advanceCount; }

    virtual std::string getName() override { return "DummyState"; }

    size_t activateCount = 0;
    size_t deactivateCount = 0;
    size_t advanceCount = 0;
    size_t updateCount = 0;
    size_t renderCount = 0;
};

//! A dummy state which automatically stops the engine when an advance occurs.
class DummyExitState : public DummyState
{
public:
    explicit DummyExitState(ECSE::Engine* engine)
        : DummyState(engine)
    {
    }

    virtual void advance() override
    {
        DummyState::advance();

        // Stop running after 9 time steps (plus the current step == 10).
        if (advanceCount >= 10)
        {
            engine->stop();
        }
    }

    virtual std::string getName() override { return "DummyExitState"; }
};

//! Initializes an engine in lockstep, no-render mode.
class EngineTest : public ::testing::Test
{
public:
    EngineTest()
    {
        engine = new ECSE::Engine(sf::Vector2i(800, 600), "", 60, true, true);
    }

    ECSE::Engine* engine;
};

//! Initializes an engine in lockstep, no-render mode, and adds a DummyState.
class EngineStateTest : public EngineTest
{
public:
    virtual void SetUp() override
    {
        state = engine->pushState<DummyState>();

        ASSERT_FALSE(state == nullptr) << "State should be returned";
    }

    DummyState* state;
};

//! Initializes an engine in lockstep, no-render mode, adds a DummyState, and takes one frame step.
class EngineStateStepTest : public EngineStateTest
{
public:
    virtual void SetUp() override
    {
        EngineStateTest::SetUp();

        engine->frameStep();
    }
};

//! Initializes an engine in lockstep mode with rendering.
class RenderEngineTest : public ::testing::Test
{
public:
    RenderEngineTest()
    {
        engine = new ECSE::Engine(sf::Vector2i(800, 600), "", 60, false, true);
    }

    ECSE::Engine* engine;
};

//! Initializes an engine in lockstep mode with rendering, adds a DummyState, and takes one frame step.
class RenderEngineStateTest : public RenderEngineTest
{
public:
    virtual void SetUp() override
    {
        state = engine->pushState<DummyState>();

        ASSERT_FALSE(state == nullptr) << "State should be returned";

        engine->frameStep();
    }

    DummyState* state;
};

// Test SetSystem because System is lacking features for a reasonable test
class DummyWorldSystem : public ECSE::SetSystem
{
public:
    explicit DummyWorldSystem(ECSE::World* world)
        : SetSystem(world)
    {
    }

    void added() override
    {
        isAdded = true;
    }

    bool isAdded = false;
    bool passChecks = true;

protected:
    bool checkRequirements(const ECSE::Entity&) const override
    {
        return passChecks;
    }
};

class DummySystem : public DummyWorldSystem
{
public:
    explicit DummySystem()
        : DummyWorldSystem(nullptr)
    {
    }
};

class DummyComponent : public ECSE::Component
{
public:
    explicit DummyComponent()
        : Component()
    {
    }
};