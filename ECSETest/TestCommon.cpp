#include "gtest/gtest.h"
#include "ECSE/Common.h"

TEST(IntervalModTest, PositivePositiveTest)
{
    ASSERT_FLOAT_EQ(2.0f, ECSE::intervalMod(6.0f, 4.0f));
}

TEST(IntervalModTest, PositiveNegativeTest)
{
    ASSERT_FLOAT_EQ(-2.0f, ECSE::intervalMod(6.0f, -4.0f));
}

TEST(IntervalModTest, NegativePositiveTest)
{
    ASSERT_FLOAT_EQ(2.0f, ECSE::intervalMod(-6.0f, 4.0f));
}

TEST(IntervalModTest, NegativeNegativeTest)
{
    ASSERT_FLOAT_EQ(-2.0f, ECSE::intervalMod(-6.0f, -4.0f));
}

TEST(LerpTest, GeneralTest)
{
    ASSERT_EQ(5.0f, ECSE::lerp(0.f, 10.f, 0.5f));
}

TEST(LerpTest, ZeroTest)
{
    ASSERT_EQ(50.0f, ECSE::lerp(50.f, 100.f, 0.f));
}

TEST(LerpTest, OneTest)
{
    ASSERT_EQ(42.0f, ECSE::lerp(12.f, 42.f, 1.f));
}

TEST(LerpTest, NegativeTest)
{
    ASSERT_EQ(-12.0f, ECSE::lerp(-20.f, -10.f, 0.8f));
}

TEST(LerpTest, NegativeAlphaTest)
{
    ASSERT_EQ(-15.0f, ECSE::lerp(-5.f, 5.f, -1.f));
}

TEST(LerpTest, LargePositiveAlphaTest)
{
    ASSERT_EQ(15.0f, ECSE::lerp(-5.f, 5.f, 2.f));
}

TEST(ClampTest, MinTest)
{
    ASSERT_EQ(-10, ECSE::clamp(-10, 10, -50));
}

TEST(ClampTest, MaxTest)
{
    ASSERT_EQ(150, ECSE::clamp(50, 150, 200));
}

TEST(ClampTest, MiddleTest)
{
    ASSERT_EQ(-50, ECSE::clamp(-100, -10, -50));
}

TEST(WrapDifferenceTest, LeftTest)
{
    ASSERT_EQ(-1.f, ECSE::wrapDifference(0.5f, 2.5f, 3.f));
}

TEST(WrapDifferenceTest, RightTest)
{
    ASSERT_EQ(1.f, ECSE::wrapDifference(2.5f, 0.5f, 3.f));
}

TEST(WrapDifferenceTest, NegativeLeftTest)
{
    ASSERT_EQ(1.f, ECSE::wrapDifference(-0.5f, -2.5f, -3.f));
}

TEST(WrapDifferenceTest, NegativeRightTest)
{
    ASSERT_EQ(-1.f, ECSE::wrapDifference(-2.5f, -0.5f, -3.f));
}

TEST(WrapDifferenceTest, BothLeftTest)
{
    ASSERT_EQ(1.f, ECSE::wrapDifference(-2.5f, 1.5f, -3.f));
}

TEST(WrapDifferenceTest, BothRightTest)
{
    ASSERT_EQ(-1.f, ECSE::wrapDifference(1.5f, -2.5f, -3.f));
}