#include "gtest/gtest.h"
#include "ECSE/VectorMath.h"
#include "ECSE/Common.h"
#include "TestUtils.h"

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
    ECSE::rotate(v, ECSE::halfPi);

    ASSERT_NEAR_TRANS(0.f, v.x);
    ASSERT_NEAR_TRANS(3.f, v.y);
}

TEST(RotateTest, Up90Test)
{
    sf::Vector2f v(0.f, 3.f);
    ECSE::rotate(v, ECSE::halfPi);

    ASSERT_NEAR_TRANS(-3.f, v.x);
    ASSERT_NEAR_TRANS(0.f, v.y);
}

TEST(RotateTest, FortyFive90Test)
{
    sf::Vector2f v(3.f, 3.f);
    ECSE::rotate(v, ECSE::halfPi);

    ASSERT_NEAR_TRANS(-3.f, v.x);
    ASSERT_NEAR_TRANS(3.f, v.y);
}

TEST(RotateTest, Right45Test)
{
    sf::Vector2f v(10.f, 0.f);
    ECSE::rotate(v, ECSE::quarterPi);

    ASSERT_NEAR_TRANS(cos(ECSE::quarterPi) * 10.f, v.x);
    ASSERT_NEAR_TRANS(sin(ECSE::quarterPi) * 10.f, v.y);
}

TEST(RotateTest, Up180Test)
{
    sf::Vector2f v(0.f, 7.5f);
    ECSE::rotate(v, ECSE::pi);

    ASSERT_NEAR_TRANS(0.f, v.x);
    ASSERT_NEAR_TRANS(-7.5f, v.y);
}

TEST(RotateTest, Left360Test)
{
    sf::Vector2f v(-200.f, 0.f);
    ECSE::rotate(v, ECSE::twoPi);

    ASSERT_NEAR_TRANS(-200.f, v.x);
    ASSERT_NEAR_TRANS(0.f, v.y);
}

TEST(GetHeadingTest, RightTest)
{
    sf::Vector2f v(5.f, 0.f);

    ASSERT_ANGLE_EQ(0.f, ECSE::getHeading(v));
}

TEST(GetHeadingTest, LeftTest)
{
    sf::Vector2f v(-5.f, 0.f);

    ASSERT_ANGLE_EQ(ECSE::pi, ECSE::getHeading(v));
}

TEST(GetHeadingTest, TwoTwentyFiveTest)
{
    sf::Vector2f v(-5.f, -5.f);

    ASSERT_ANGLE_EQ(ECSE::pi + ECSE::quarterPi, ECSE::getHeading(v));
}

TEST(GetSqrMagnitudeTest, OneTest)
{
    sf::Vector2f v(1.f, 0.f);

    ASSERT_FLOAT_EQ(1.f, ECSE::getSqrMagnitude(v));
}

TEST(GetSqrMagnitudeTest, FiveTest)
{
    sf::Vector2f v(0.f, 5.f);

    ASSERT_FLOAT_EQ(25.f, ECSE::getSqrMagnitude(v));
}

TEST(GetSqrMagnitudeTest, BothTest)
{
    sf::Vector2f v(10.f, 8.f);

    ASSERT_FLOAT_EQ(164.f, ECSE::getSqrMagnitude(v));
}

TEST(GetDotProductTest, GeneralTest)
{
    sf::Vector2f a(1.f, 2.f);
    sf::Vector2f b(4.f, 5.f);

    ASSERT_FLOAT_EQ(14.f, ECSE::getDotProduct(a, b));
}

TEST(GetDotProductTest, PerpendicularTest)
{
    sf::Vector2f a(1.f, 0.f);
    sf::Vector2f b(0.f, 1.f);

    ASSERT_FLOAT_EQ(0.f, ECSE::getDotProduct(a, b));
}

TEST(GetDotProductTest, ParallelTest)
{
    sf::Vector2f a(5.f, 0.f);
    sf::Vector2f b(5.f, 0.f);

    ASSERT_FLOAT_EQ(25.f, ECSE::getDotProduct(a, b));
}

TEST(SetHeadingTest, DifferentTest)
{
    sf::Vector2f v(15.f, 0.f);
    float newAngle = ECSE::halfPi + ECSE::quarterPi;
    ECSE::setHeading(v, newAngle);

    ASSERT_ANGLE_EQ(newAngle, ECSE::getHeading(v));
    ASSERT_NEAR_TRANS(225.f, ECSE::getSqrMagnitude(v));
}

TEST(SetHeadingTest, SameTest)
{
    sf::Vector2f v(-5.f, 5.f);
    float newAngle = ECSE::halfPi + ECSE::quarterPi;
    ECSE::setHeading(v, newAngle);

    ASSERT_ANGLE_EQ(newAngle, ECSE::getHeading(v));
    ASSERT_NEAR_TRANS(50.f, ECSE::getSqrMagnitude(v));
}

TEST(NormalizeTest, GeneralTest)
{
    sf::Vector2f v(22.f, 37.f);
    float heading = ECSE::getHeading(v);

    ECSE::normalize(v);

    ASSERT_ANGLE_EQ(heading, ECSE::getHeading(v));
    ASSERT_NEAR_TRANS(1.f, ECSE::getSqrMagnitude(v));
}

TEST(SetMagnitudeTest, GeneralTest)
{
    sf::Vector2f v(11.f, 4.f);
    float heading = ECSE::getHeading(v);
    float newMagnitude = 3.f;

    ECSE::setMagnitude(v, newMagnitude);

    ASSERT_ANGLE_EQ(heading, ECSE::getHeading(v));
    ASSERT_NEAR_TRANS(newMagnitude, ECSE::getMagnitude(v));
}

TEST(ProjectTest, GeneralTest)
{
    sf::Vector2f a(5.f, 5.f);
    sf::Vector2f b(1.f, 0.f);

    ECSE::project(a, b);

    ASSERT_ANGLE_EQ(0.f, ECSE::getHeading(a));
    ASSERT_NEAR_TRANS(5.f, ECSE::getMagnitude(a));
}

TEST(ProjectTest, PerpendicularTest)
{
    sf::Vector2f a(0.f, -1.f);
    sf::Vector2f b(1.f, 0.f);

    ECSE::project(a, b);

    ASSERT_NEAR_TRANS(0.f, ECSE::getMagnitude(a));
}

TEST(ProjectTest, ParallelTest)
{
    sf::Vector2f a(1.f, 0.f);
    sf::Vector2f b(1.f, 0.f);

    ECSE::project(a, b);

    ASSERT_ANGLE_EQ(0.f, ECSE::getHeading(a));
    ASSERT_NEAR_TRANS(1.f, ECSE::getMagnitude(a));
}

TEST(RejectTest, GeneralTest)
{
    sf::Vector2f a(5.f, 5.f);
    sf::Vector2f b(1.f, 0.f);

    ECSE::reject(a, b);

    ASSERT_NEAR_TRANS(5.f, ECSE::getMagnitude(a));
    ASSERT_NEAR_TRANS(0.f, ECSE::getDotProduct(a, b));
}

TEST(RejectTest, PerpendicularTest)
{
    sf::Vector2f a(0.f, -1.f);
    sf::Vector2f b(1.f, 0.f);

    ECSE::reject(a, b);

    ASSERT_NEAR_TRANS(1.f, ECSE::getMagnitude(a));
    ASSERT_NEAR_TRANS(0.f, ECSE::getDotProduct(a, b));
}

TEST(RejectTest, ParallelTest)
{
    sf::Vector2f a(1.f, 0.f);
    sf::Vector2f b(1.f, 0.f);

    ECSE::reject(a, b);

    ASSERT_NEAR_TRANS(0.f, ECSE::getDotProduct(a, b));
}