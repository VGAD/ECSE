#include "gtest/gtest.h"
#include "ECSE/InputManager.h"

class InputManagerTest : public ::testing::Test
{
public:
    ECSE::InputManager manager;

    InputManagerTest()
    {
        manager.setRequireFocus(false);
    }
};

TEST_F(InputManagerTest, TestIsBound)
{
    std::function<bool()> fn = []() { return false; };
    manager.bindInput(0, 0, fn);

    ASSERT_TRUE(manager.isBound(0));
}

TEST_F(InputManagerTest, TestBoolInput)
{
    bool value;

    std::function<bool()> fn = [&value]() { return value; };
    manager.bindInput(0, 0, fn);

    value = false;

    ASSERT_EQ(0, manager.getIntValue(0));
    ASSERT_FLOAT_EQ(0.f, manager.getFloatValue(0));

    value = true;

    ASSERT_EQ(1, manager.getIntValue(0));
    ASSERT_FLOAT_EQ(1.f, manager.getFloatValue(0));
}

TEST_F(InputManagerTest, TestIntInput)
{
    int value;

    std::function<int()> fn = [&value]() { return value; };
    manager.bindInput(0, 0, fn);

    value = 0;

    ASSERT_EQ(0, manager.getIntValue(0));
    ASSERT_FLOAT_EQ(0.f, manager.getFloatValue(0));

    value = 1;

    ASSERT_EQ(1, manager.getIntValue(0));
    ASSERT_FLOAT_EQ(1.f, manager.getFloatValue(0));

    value = -1;

    ASSERT_EQ(-1, manager.getIntValue(0));
    ASSERT_FLOAT_EQ(-1.f, manager.getFloatValue(0));
}

TEST_F(InputManagerTest, TestFloatInput)
{
    float value;

    std::function<float()> fn = [&value]() { return value; };
    manager.bindInput(0, 0, fn);

    value = 0.f;

    ASSERT_EQ(0, manager.getIntValue(0));
    ASSERT_FLOAT_EQ(0.f, manager.getFloatValue(0));

    value = 1.f;

    ASSERT_EQ(1, manager.getIntValue(0));
    ASSERT_FLOAT_EQ(1.f, manager.getFloatValue(0));

    value = -1.f;

    ASSERT_EQ(-1, manager.getIntValue(0));
    ASSERT_FLOAT_EQ(-1.f, manager.getFloatValue(0));
}

TEST_F(InputManagerTest, TestSensitivity)
{
    float value;
    float precision = 1 << ECSE_INPUT_PRECISION;

    std::function<float()> fn = [&value]() { return value; };
    manager.bindInput(0, 0, fn, 0.3f);

    value = 0.1f;

    ASSERT_EQ(0, manager.getIntValue(0));
    ASSERT_NEAR(0.f, manager.getFloatValue(0), precision);

    value = 0.4f;

    ASSERT_EQ(1, manager.getIntValue(0));
    ASSERT_NEAR(0.4f, manager.getFloatValue(0), precision);

    value = -0.4f;

    ASSERT_EQ(-1, manager.getIntValue(0));
    ASSERT_NEAR(-0.4f, manager.getFloatValue(0), precision);
}

TEST_F(InputManagerTest, TestMultipleInputs)
{
    int val1, val2;

    std::function<int()> fn1 = [&val1]() { return val1; };
    manager.bindInput(0, 0, fn1);

    std::function<int()> fn2 = [&val2]() { return val2; };
    manager.bindInput(1, 0, fn2);

    val1 = 0;
    val2 = 0;

    ASSERT_EQ(0, manager.getIntValue(0));
    ASSERT_EQ(0, manager.getIntValue(1));

    val1 = 1;
    val2 = 0;

    ASSERT_EQ(1, manager.getIntValue(0));
    ASSERT_EQ(0, manager.getIntValue(1));

    val1 = 1;
    val2 = 1;

    ASSERT_EQ(1, manager.getIntValue(0));
    ASSERT_EQ(1, manager.getIntValue(1));

    val1 = 0;
    val2 = 1;

    ASSERT_EQ(0, manager.getIntValue(0));
    ASSERT_EQ(1, manager.getIntValue(1));
}

TEST_F(InputManagerTest, TestSwitchModes)
{
    int val1, val2;

    std::function<int()> fn1 = [&val1]() { return val1; };
    manager.bindInput(0, 0, fn1);

    std::function<int()> fn2 = [&val2]() { return val2; };
    manager.bindInput(0, 1, fn2);

    val1 = 0;
    val2 = 0;

    manager.setInputMode(0);
    ASSERT_EQ(0, manager.getIntValue(0));
    manager.setInputMode(1);
    ASSERT_EQ(0, manager.getIntValue(0));

    val1 = 1;
    val2 = 0;

    manager.setInputMode(0);
    ASSERT_EQ(1, manager.getIntValue(0));
    manager.setInputMode(1);
    ASSERT_EQ(0, manager.getIntValue(0));

    val1 = 1;
    val2 = 1;

    manager.setInputMode(0);
    ASSERT_EQ(1, manager.getIntValue(0));
    manager.setInputMode(1);
    ASSERT_EQ(1, manager.getIntValue(0));

    val1 = 0;
    val2 = 1;

    manager.setInputMode(0);
    ASSERT_EQ(0, manager.getIntValue(0));
    manager.setInputMode(1);
    ASSERT_EQ(1, manager.getIntValue(0));
}

TEST_F(InputManagerTest, TestMultipleModes)
{
    int val1, val2;

    std::function<int()> fn1 = [&val1]() { return val1; };
    manager.bindInput(0, 0, fn1);

    std::function<int()> fn2 = [&val2]() { return val2; };
    manager.bindInput(0, 1, fn2);

    val1 = 0;
    val2 = 0;

    ASSERT_EQ(0, manager.getIntValue(0, 0));
    ASSERT_EQ(0, manager.getIntValue(0, 1));

    val1 = 1;
    val2 = 0;

    ASSERT_EQ(1, manager.getIntValue(0, 0));
    ASSERT_EQ(0, manager.getIntValue(0, 1));

    val1 = 1;
    val2 = 1;

    ASSERT_EQ(1, manager.getIntValue(0, 0));
    ASSERT_EQ(1, manager.getIntValue(0, 1));

    val1 = 0;
    val2 = 1;

    ASSERT_EQ(0, manager.getIntValue(0, 0));
    ASSERT_EQ(1, manager.getIntValue(0, 1));
}