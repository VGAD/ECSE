#include "gtest/gtest.h"
#include "ECSE/VectorMath.h"
#include "ECSE/Common.h"

#define ASSERT_ANGLE_EQ(val1, val2) \
    ASSERT_NEAR(ECSE::wrapDifference(val1, val2, 2.f * ECSE::pi), 0.f, ECSE::degToRad(0.01f))

#define ASSERT_NEAR_VECMATH(val1, val2) \
    ASSERT_NEAR(val1, val2, 0.0001f)

TEST(Rotate90Test, RightTest)
{
    sf::Vector2f v(3.f, 0.f);
    ECSE::rotate90(v);

    ASSERT_FLOAT_EQ(0.f, v.x);
    ASSERT_FLOAT_EQ(3.f, v.y);
}

TEST(Rotate90Test, UpTest)
{
    sf::Vector2f v(0.f, 3.f);
    ECSE::rotate90(v);

    ASSERT_FLOAT_EQ(-3.f, v.x);
    ASSERT_FLOAT_EQ(0.f, v.y);
}

TEST(Rotate90Test, FortyFiveTest)
{
    sf::Vector2f v(3.f, 3.f);
    ECSE::rotate90(v);

    ASSERT_FLOAT_EQ(-3.f, v.x);
    ASSERT_FLOAT_EQ(3.f, v.y);
}

TEST(RotateTest, Right90Test)
{
    sf::Vector2f v(3.f, 0.f);
    ECSE::rotate(v, ECSE::half_pi);

    ASSERT_NEAR_VECMATH(0.f, v.x);
    ASSERT_NEAR_VECMATH(3.f, v.y);
}

TEST(RotateTest, Up90Test)
{
    sf::Vector2f v(0.f, 3.f);
    ECSE::rotate(v, ECSE::half_pi);

    ASSERT_NEAR_VECMATH(-3.f, v.x);
    ASSERT_NEAR_VECMATH(0.f, v.y);
}

TEST(RotateTest, FortyFive90Test)
{
    sf::Vector2f v(3.f, 3.f);
    ECSE::rotate(v, ECSE::half_pi);

    ASSERT_NEAR_VECMATH(-3.f, v.x);
    ASSERT_NEAR_VECMATH(3.f, v.y);
}

TEST(RotateTest, Right45Test)
{
    sf::Vector2f v(10.f, 0.f);
    ECSE::rotate(v, ECSE::quarter_pi);

    ASSERT_NEAR_VECMATH(cos(ECSE::quarter_pi) * 10.f, v.x);
    ASSERT_NEAR_VECMATH(sin(ECSE::quarter_pi) * 10.f, v.y);
}

TEST(RotateTest, Up180Test)
{
    sf::Vector2f v(0.f, 7.5f);
    ECSE::rotate(v, ECSE::pi);

    ASSERT_NEAR_VECMATH(0.f, v.x);
    ASSERT_NEAR_VECMATH(-7.5f, v.y);
}

TEST(RotateTest, Left360Test)
{
    sf::Vector2f v(-200.f, 0.f);
    ECSE::rotate(v, ECSE::two_pi);

    ASSERT_NEAR_VECMATH(-200.f, v.x);
    ASSERT_NEAR_VECMATH(0.f, v.y);
}