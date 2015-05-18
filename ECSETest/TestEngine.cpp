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