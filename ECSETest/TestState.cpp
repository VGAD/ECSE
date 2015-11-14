#include "gtest/gtest.h"
#include "TestFixtures.h"
#include "ECSE/State.h"

class StateTest : public ::testing::Test
{
public:
    StateTest()
        : state(nullptr)
    {}

    DummyState state;
};

TEST_F(StateTest, TestCoroutine)
{
    int x = 0;
    state.startSideRoutine([&x]()
    {
        ++x;

        return false;
    });

    state.update(sf::Time());
    state.update(sf::Time());
    state.update(sf::Time());

    ASSERT_EQ(3, x);
}

TEST_F(StateTest, TestMultipleCoroutine)
{
    int x = 0;
    int y = 0;

    state.startSideRoutine([&x]()
    {
        x += 1;

        return false;
    });

    state.startSideRoutine([&y]()
    {
        y += 2;

        return false;
    });

    state.update(sf::Time());
    state.update(sf::Time());
    state.update(sf::Time());

    ASSERT_EQ(3, x);
    ASSERT_EQ(6, y);
}

TEST_F(StateTest, TestStopCoroutine)
{
    int x = 0;
    state.startSideRoutine([&x]()
    {
        ++x;

        if (x == 2) return true;

        return false;
    });

    state.update(sf::Time());
    state.update(sf::Time());
    state.update(sf::Time());

    ASSERT_EQ(2, x);
}

TEST_F(StateTest, TestStopMultipleCoroutines)
{
    int x = 0;
    int y = 0;
    int z = 0;

    state.startSideRoutine([&x]()
    {
        x += 1;

        return true;
    });

    state.startSideRoutine([&y]()
    {
        y += 2;

        if (y == 4) return true;

        return false;
    });

    state.startSideRoutine([&z]()
    {
        z += 3;

        return false;
    });

    state.update(sf::Time());
    state.update(sf::Time());
    state.update(sf::Time());

    ASSERT_EQ(1, x);
    ASSERT_EQ(4, y);
    ASSERT_EQ(9, z);
}