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

class InputManagerRunTest : public InputManagerTest
{
public:
    ECSE::InputManager manager;

    InputManagerRunTest()
    {
        std::function<bool()> boolfn = []() { return false; };
        std::function<int()> intfn = []() { return 0; };
        std::function<float()> floatfn = []() { return 0.0f; };

        manager.bindInput(0, 0, boolfn);
        manager.bindInput(1, 0, intfn);
        manager.bindInput(2, 0, floatfn, 0.2f);

        manager.bindInput(0, 1, boolfn);
        manager.bindInput(1, 1, intfn);
        manager.bindInput(2, 1, floatfn, 0.4f);
    }

    void runLoop()
    {
        a = 0;
        b = 0;
        c = 0;

        for (int i = 0; i < 1000; ++i)
        {
            manager.update();

            if (rand() % 100 > 10)
            {
                manager.setInputMode(manager.getInputMode() == 0 ? 1 : 0);
            }

            if (manager.getIntValue(0) > 0)
            {
                a += 2;
                b -= 1;
            }

            if (manager.getFloatValue(1) >= 0.7)
            {
                b += 3;
                c -= 2;
            }

            if (manager.getIntValue(2) < 1)
            {
                a += 1;
                c += 4;
            }
        }
    }

    int a, b, c;
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
    manager.update();

    ASSERT_EQ(0, manager.getIntValue(0));
    ASSERT_FLOAT_EQ(0.f, manager.getFloatValue(0));

    value = true;
    manager.update();

    ASSERT_EQ(1, manager.getIntValue(0));
    ASSERT_FLOAT_EQ(1.f, manager.getFloatValue(0));
}

TEST_F(InputManagerTest, TestIntInput)
{
    int value;

    std::function<int()> fn = [&value]() { return value; };
    manager.bindInput(0, 0, fn);

    value = 0;
    manager.update();

    ASSERT_EQ(0, manager.getIntValue(0));
    ASSERT_FLOAT_EQ(0.f, manager.getFloatValue(0));

    value = 1;
    manager.update();

    ASSERT_EQ(1, manager.getIntValue(0));
    ASSERT_FLOAT_EQ(1.f, manager.getFloatValue(0));

    value = -1;
    manager.update();

    ASSERT_EQ(-1, manager.getIntValue(0));
    ASSERT_FLOAT_EQ(-1.f, manager.getFloatValue(0));
}

TEST_F(InputManagerTest, TestFloatInput)
{
    float value;

    std::function<float()> fn = [&value]() { return value; };
    manager.bindInput(0, 0, fn);

    value = 0.f;
    manager.update();

    ASSERT_EQ(0, manager.getIntValue(0));
    ASSERT_FLOAT_EQ(0.f, manager.getFloatValue(0));

    value = 1.f;
    manager.update();

    ASSERT_EQ(1, manager.getIntValue(0));
    ASSERT_FLOAT_EQ(1.f, manager.getFloatValue(0));

    value = -1.f;
    manager.update();

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
    manager.update();

    ASSERT_EQ(0, manager.getIntValue(0));
    ASSERT_NEAR(0.f, manager.getFloatValue(0), precision);

    value = 0.4f;
    manager.update();

    ASSERT_EQ(1, manager.getIntValue(0));
    ASSERT_NEAR(0.4f, manager.getFloatValue(0), precision);

    value = -0.4f;
    manager.update();

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
    manager.update();

    ASSERT_EQ(0, manager.getIntValue(0));
    ASSERT_EQ(0, manager.getIntValue(1));

    val1 = 1;
    val2 = 0;
    manager.update();

    ASSERT_EQ(1, manager.getIntValue(0));
    ASSERT_EQ(0, manager.getIntValue(1));

    val1 = 1;
    val2 = 1;
    manager.update();

    ASSERT_EQ(1, manager.getIntValue(0));
    ASSERT_EQ(1, manager.getIntValue(1));

    val1 = 0;
    val2 = 1;
    manager.update();

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
    manager.update();

    manager.setInputMode(0);
    ASSERT_EQ(0, manager.getIntValue(0));
    manager.setInputMode(1);
    ASSERT_EQ(0, manager.getIntValue(0));

    val1 = 1;
    val2 = 0;
    manager.update();

    manager.setInputMode(0);
    ASSERT_EQ(1, manager.getIntValue(0));
    manager.setInputMode(1);
    ASSERT_EQ(0, manager.getIntValue(0));

    val1 = 1;
    val2 = 1;
    manager.update();

    manager.setInputMode(0);
    ASSERT_EQ(1, manager.getIntValue(0));
    manager.setInputMode(1);
    ASSERT_EQ(1, manager.getIntValue(0));

    val1 = 0;
    val2 = 1;
    manager.update();

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
    manager.update();

    ASSERT_EQ(0, manager.getIntValue(0, 0));
    ASSERT_EQ(0, manager.getIntValue(0, 1));

    val1 = 1;
    val2 = 0;
    manager.update();

    ASSERT_EQ(1, manager.getIntValue(0, 0));
    ASSERT_EQ(0, manager.getIntValue(0, 1));

    val1 = 1;
    val2 = 1;
    manager.update();

    ASSERT_EQ(1, manager.getIntValue(0, 0));
    ASSERT_EQ(1, manager.getIntValue(0, 1));

    val1 = 0;
    val2 = 1;
    manager.update();

    ASSERT_EQ(0, manager.getIntValue(0, 0));
    ASSERT_EQ(1, manager.getIntValue(0, 1));
}

TEST_F(InputManagerRunTest, TestMonkey)
{
    manager.startMonkeyMode();
    runLoop();

    // a always increases, so if the monkey is at all good at pressing buttons this should work
    ASSERT_FALSE(a == 0, "Monkey may not be pressing buttons");

    // This should be pretty unlikely
    ASSERT_FALSE(a == b && b == c);
}

TEST_F(InputManagerRunTest, TestDemo)
{
    std::stringstream stream;

    manager.startMonkeyMode();
    manager.startRecording(stream);
    runLoop();
    manager.stopRecording();

    int prevA = a;
    int prevB = b;
    int prevC = c;

    manager.playDemo(stream);
    runLoop();

    ASSERT_EQ(prevA, a);
    ASSERT_EQ(prevB, b);
    ASSERT_EQ(prevC, c);

    ASSERT_FALSE(manager.isPlayingDemo());
}