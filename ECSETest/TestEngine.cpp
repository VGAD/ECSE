#include "TestFixtures.h"

TEST_F(EngineStateStepTest, ActivateTest)
{
    ASSERT_EQ(1, state->activateCount) << "Activation should only occur once";
}

TEST_F(EngineStateStepTest, DeactivateTest)
{
    ASSERT_EQ(0, state->deactivateCount) << "State is still on top, so should not be deactivated";
}

TEST_F(EngineStateStepTest, UpdateAdvanceTest)
{
    ASSERT_GT(state->updateCount, size_t(0)) << "State should update at least once";
    ASSERT_GT(state->advanceCount, size_t(0)) << "State should advance at least once";
    ASSERT_EQ(state->updateCount - 1, state->advanceCount) << "State should have advanced one less time than updated";
}

TEST_F(EngineStateStepTest, TickTest)
{
    ASSERT_EQ(state->advanceCount, engine->getTimeSteps()) << "Time step count should equal advance count";
}

TEST_F(RenderEngineStateTest, FrameTest)
{
    ASSERT_EQ(state->renderCount, engine->getFrames()) << "Frame count should equal render count";
}

TEST_F(RenderEngineStateTest, RenderTest)
{
    ASSERT_GT(state->renderCount, size_t(0)) << "State should have rendered at least one frame";
}

TEST_F(EngineStateTest, MultiFrameStepTest)
{
    size_t stepCount = 3;
    for (size_t i = 0; i < stepCount; ++i)
    {
        engine->frameStep();
    }

    ASSERT_EQ(stepCount, engine->getFrames()) << "Engine should simulate " << stepCount << " frames";
    ASSERT_EQ(stepCount, engine->getTimeSteps()) << "Engine should simulate " << stepCount << " time steps";
}

TEST_F(EngineTest, RunTest)
{
    engine->pushState<DummyExitState>();
    engine->run();

    ASSERT_EQ(10, engine->getTimeSteps()) << "Engine should simulate 10 time steps";
}

TEST_F(EngineTest, NoStateTest)
{
    ASSERT_THROW(engine->getActiveState(), std::runtime_error) << "Stateless engine should throw exception";
}

TEST_F(EngineTest, PushStateTest)
{
    DummyState* a = engine->pushState<DummyState>();

    ASSERT_THROW(engine->getActiveState(), std::runtime_error) << "Engine should still have no active state";

    engine->frameStep();

    ASSERT_EQ(&engine->getActiveState(), a) << "State should be added";
}

TEST_F(EngineTest, ChangeStateTest)
{
    DummyState* a = engine->pushState<DummyState>();
    engine->frameStep();

    ASSERT_EQ(1, a->advanceCount) << "First state should have advanced once";
    ASSERT_EQ(2, a->updateCount) << "First state should have updated once";

    DummyState* b = engine->pushState<DummyExitState>();
    engine->run();

    ASSERT_EQ(2, a->advanceCount) << "First state should have advanced one more time";
    ASSERT_EQ(2, a->updateCount) << "First state should not have updated again";

    ASSERT_EQ(10, b->advanceCount) << "Second state should have advanced 10 times";
    ASSERT_EQ(11, b->updateCount) << "Second state should have updated one extra time to be ready for next time step";
}

TEST_F(EngineTest, PopStateTest)
{
    DummyState* a = engine->pushState<DummyExitState>();
    engine->frameStep();

    DummyState* b = engine->pushState<DummyExitState>();
    engine->run();

    ASSERT_EQ(10, b->advanceCount) << "Second state should have advanced 10 times";
    ASSERT_EQ(11, b->updateCount) << "Second state should have updated one extra time to be ready for next time step";

    engine->popState();
    engine->run();

    ASSERT_EQ(2, a->advanceCount) << "First state should have advanced twice";
    ASSERT_EQ(3, a->updateCount) << "First state should have updated one extra time to be ready for next time step";
}