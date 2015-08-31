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
    InputManagerRunTest()
    {
        resetValues();

        std::function<bool()> boolfn = []() { return false; };
        std::function<int8_t()> intfn = []() { return 0; };
        std::function<float()> floatfn = []() { return 0.0f; };
        std::function<bool()> dynamicfn = [&]() { return dynamicValue; };

        manager.bindInput(0, 0, boolfn);
        manager.bindInput(1, 0, intfn);
        manager.bindInput(2, 0, floatfn, 0.2f);
        manager.bindInput(3, 0, dynamicfn);

        manager.bindInput(0, 1, boolfn);
        manager.bindInput(1, 1, intfn);
        manager.bindInput(2, 1, floatfn, 0.4f);
        manager.bindInput(3, 1, dynamicfn);
    }

    void resetValues()
    {
        a = 0;
        b = 0;
        c = 0;
        d = 0;
        e = 0;
        frames = 0;
        mouseAvg = sf::Vector2i();
    }

    void runLoop(int length)
    {
        for (int i = 0; i < length; ++i)
        {
            manager.update();

            bool anyPressed = false;
            if (manager.getIntValue(0) > 0)
            {
                a += 1;
                anyPressed = true;
            }

            if (manager.getFloatValue(1) >= 0.7)
            {
                b += 1;
                anyPressed = true;
            }

            if (manager.getIntValue(2) < 1)
            {
                c += 1;
                anyPressed = true;
            }

            if (manager.getIntValue(3) > 0)
            {
                d += 1;
                anyPressed = true;
            }

            if (!anyPressed)
            {
                e += 1;
            }

            mouseAvg = (mouseAvg * frames + manager.getMousePosition()) / (frames + 1);

            ++frames;
        }
    }

    int a, b, c, d, e;
    int frames;
    bool dynamicValue;
    sf::Vector2i mouseAvg;
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

    std::function<int8_t()> fn = [&value]() { return value; };
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

TEST_F(InputManagerTest, TestDelta)
{
    int value;

    std::function<int8_t()> fn = [&value]() { return value; };
    manager.bindInput(0, 0, fn);

    value = 0;
    manager.update();

    ASSERT_EQ(0, manager.getIntDelta(0));
    ASSERT_FLOAT_EQ(0.f, manager.getFloatDelta(0));

    value = 1;
    manager.update();

    ASSERT_EQ(1, manager.getIntDelta(0));
    ASSERT_FLOAT_EQ(1.f, manager.getFloatDelta(0));

    value = -1;
    manager.update();

    ASSERT_EQ(-2, manager.getIntDelta(0));
    ASSERT_FLOAT_EQ(-2.f, manager.getFloatDelta(0));
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

    std::function<int8_t()> fn1 = [&val1]() { return val1; };
    manager.bindInput(0, 0, fn1);

    std::function<int8_t()> fn2 = [&val2]() { return val2; };
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

    std::function<int8_t()> fn1 = [&val1]() { return val1; };
    manager.bindInput(0, 0, fn1);

    std::function<int8_t()> fn2 = [&val2]() { return val2; };
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

    std::function<int8_t()> fn1 = [&val1]() { return val1; };
    manager.bindInput(0, 0, fn1);

    std::function<int8_t()> fn2 = [&val2]() { return val2; };
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
    runLoop(1000);

    // These always increase, so if the monkey is at all good at pressing buttons this should work
    ASSERT_FALSE(a == 0) << "Monkey may not be pressing buttons";
    ASSERT_FALSE(b == 0) << "Monkey may not be pressing buttons";
    ASSERT_FALSE(c == 0) << "Monkey may not be pressing buttons";

    // This should be pretty unlikely
    ASSERT_FALSE(a == b && b == c);
}

TEST_F(InputManagerRunTest, TestDemo)
{
    std::stringstream stream;

    manager.startMonkeyMode();
    manager.startRecording(stream);
    runLoop(1000);
    manager.stopRecording();
    manager.stopMonkeyMode();

    int prevA = a;
    int prevB = b;
    int prevC = c;
    int prevE = e;

    resetValues();
    manager.playDemo(stream);
    runLoop(1000);

    ASSERT_EQ(prevA, a);
    ASSERT_EQ(prevB, b);
    ASSERT_EQ(prevC, c);
    ASSERT_EQ(prevE, e);

    ASSERT_FALSE(manager.isPlayingDemo());
}

TEST_F(InputManagerRunTest, TestHoldDemo)
{
    std::stringstream stream;

    dynamicValue = true;
    manager.startRecording(stream);
    runLoop(1000);
    manager.stopRecording();

    int prevD = d;


    // Real value is now false
    dynamicValue = false;
    manager.update();


    resetValues();
    manager.playDemo(stream);
    runLoop(1000);


    ASSERT_EQ(prevD, d) << "Demo cancelled early";
}

TEST_F(InputManagerRunTest, TestMouseDemo)
{
    std::stringstream stream;

    manager.startMonkeyMode();
    manager.startRecording(stream, true);
    runLoop(1000);
    manager.stopRecording();
    manager.stopMonkeyMode();

    auto prevMouseAvg = mouseAvg;

    resetValues();
    manager.playDemo(stream);
    runLoop(1000);

    ASSERT_EQ(prevMouseAvg.x, mouseAvg.x);
    ASSERT_EQ(prevMouseAvg.y, mouseAvg.y);
}

TEST_F(InputManagerRunTest, TestModeSwitchDemo)
{
    std::stringstream stream;

    manager.startMonkeyMode();
    manager.startRecording(stream);

    for (int i = 0; i < 100; ++i)
    {
        runLoop(10);

        if (rand() % 100 > 90)
        {
            manager.setInputMode(manager.getInputMode() == 0 ? 1 : 0);
        }
    }

    manager.stopRecording();
    manager.stopMonkeyMode();

    auto str = stream.str();

    int prevA = a;
    int prevB = b;
    int prevC = c;
    int prevE = e;

    resetValues();
    manager.playDemo(stream);
    runLoop(1000);

    ASSERT_EQ(prevA, a);
    ASSERT_EQ(prevB, b);
    ASSERT_EQ(prevC, c);
    ASSERT_EQ(prevE, e);
}

TEST_F(InputManagerRunTest, TestInputGapDemo)
{
    std::stringstream stream;

    manager.startMonkeyMode();
    manager.startRecording(stream);

    for (int i = 0; i < 5; ++i)
    {
        runLoop(500);

        if (manager.isInMonkeyMode()) manager.stopMonkeyMode();
        else manager.startMonkeyMode();
    }

    manager.stopRecording();
    manager.stopMonkeyMode();

    int prevA = a;
    int prevB = b;
    int prevC = c;
    int prevE = e;

    resetValues();
    manager.playDemo(stream);
    runLoop(2500);

    ASSERT_EQ(prevA, a);
    ASSERT_EQ(prevB, b);
    ASSERT_EQ(prevC, c);
    ASSERT_EQ(prevE, e);
}

TEST_F(InputManagerRunTest, TestModeSwitchInputGapDemo)
{
    std::stringstream stream;

    manager.startMonkeyMode();
    manager.startRecording(stream);

    for (int i = 0; i < 20; ++i)
    {
        runLoop(50);

        if (rand() % 100 > 90)
        {
            manager.setInputMode(manager.getInputMode() == 0 ? 1 : 0);
        }

        if (i % 10 == 0)
        {
            if (manager.isInMonkeyMode()) manager.stopMonkeyMode();
            else manager.startMonkeyMode();
        }
    }

    manager.stopRecording();
    manager.stopMonkeyMode();

    int prevA = a;
    int prevB = b;
    int prevC = c;
    int prevE = e;

    resetValues();
    manager.playDemo(stream);
    runLoop(1000);

    ASSERT_EQ(prevA, a);
    ASSERT_EQ(prevB, b);
    ASSERT_EQ(prevC, c);
    ASSERT_EQ(prevE, e);
}