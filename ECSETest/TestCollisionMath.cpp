#include "gtest/gtest.h"
#include "ECSE/CollisionMath.h"

// cross3 == 0
TEST(FindLineIntersectionTest, ParallelTest)
{
    auto result = ECSE::findLineIntersection(sf::Vector2f(0.f, 0.f), sf::Vector2f(10.f, 0.f),
                                             sf::Vector2f(2.f, 3.f), sf::Vector2f(12.f, 3.f));

    ASSERT_FALSE(result.intersection);
}

// t < 0
TEST(FindLineIntersectionTest, StrictLowTTest)
{
    auto result = ECSE::findLineIntersection(sf::Vector2f(10.f, 5.f), sf::Vector2f(10.f, 15.f),
                                             sf::Vector2f(5.f, 0.f), sf::Vector2f(15.f, 0.f));

    ASSERT_TRUE(result.intersection);
    ASSERT_FALSE(result.strictIntersection);
}

// t > 1
TEST(FindLineIntersectionTest, StrictHighTTest)
{
    auto result = ECSE::findLineIntersection(sf::Vector2f(10.f, 5.f), sf::Vector2f(10.f, 15.f),
                                             sf::Vector2f(5.f, 20.f), sf::Vector2f(15.f, 20.f));

    ASSERT_TRUE(result.intersection);
    ASSERT_FALSE(result.strictIntersection);
}

// u < 0
TEST(FindLineIntersectionTest, StrictLowUTest)
{
    auto result = ECSE::findLineIntersection(sf::Vector2f(5.f, 0.f), sf::Vector2f(15.f, 0.f),
                                             sf::Vector2f(10.f, 5.f), sf::Vector2f(10.f, 15.f));

    ASSERT_TRUE(result.intersection);
    ASSERT_FALSE(result.strictIntersection);
}

// u > 1
TEST(FindLineIntersectionTest, StrictHighUTest)
{
    auto result = ECSE::findLineIntersection(sf::Vector2f(5.f, 20.f), sf::Vector2f(15.f, 20.f),
                                             sf::Vector2f(10.f, 5.f), sf::Vector2f(10.f, 15.f));

    ASSERT_TRUE(result.intersection);
    ASSERT_FALSE(result.strictIntersection);
}

TEST(FindLineIntersectionTest, StrictMissTest)
{
    auto result = ECSE::findLineIntersection(sf::Vector2f(0.f, 10.f), sf::Vector2f(10.f, 10.f),
                                             sf::Vector2f(5.f, 5.f), sf::Vector2f(35.f, 15.f));

    ASSERT_TRUE(result.intersection);
    ASSERT_FALSE(result.strictIntersection);
}

TEST(FindLineIntersectionTest, StrictIntersectTest)
{
    auto result = ECSE::findLineIntersection(sf::Vector2f(0.f, 10.f), sf::Vector2f(10.f, 10.f),
                                             sf::Vector2f(6.f, 5.f), sf::Vector2f(6.f, 15.f));

    ASSERT_TRUE(result.intersection);
    ASSERT_TRUE(result.strictIntersection);
    ASSERT_FLOAT_EQ(result.t, 0.6f);
    ASSERT_FLOAT_EQ(result.u, 0.5f);
}

TEST(FindLineIntersectionTest, LooseIntersectTest)
{
    auto result = ECSE::findLineIntersection(sf::Vector2f(10.f, 5.f), sf::Vector2f(10.f, 15.f),
                                             sf::Vector2f(5.f, 20.f), sf::Vector2f(15.f, 20.f));

    ASSERT_TRUE(result.intersection);
    ASSERT_FALSE(result.strictIntersection);
    ASSERT_FLOAT_EQ(result.t, 1.5f);
    ASSERT_FLOAT_EQ(result.u, 0.5f);
}

TEST(ProjectPointOntoLineTest, GeneralTest)
{
    sf::Vector2f point(2.f, 2.f);

    auto result = ECSE::projectPointOntoLine(point,
                                             sf::Vector2f(0.f, 0.f),
                                             sf::Vector2f(5.f, 0.f));

    ASSERT_FLOAT_EQ(0.4f, result);
    ASSERT_FLOAT_EQ(2.f, point.x);
    ASSERT_FLOAT_EQ(0.f, point.y);
}

TEST(ProjectPointOntoLineTest, OffsetTest)
{
    sf::Vector2f point(7.f, 7.f);

    auto result = ECSE::projectPointOntoLine(point,
                                             sf::Vector2f(5.f, 5.f),
                                             sf::Vector2f(10.f, 5.f));

    ASSERT_FLOAT_EQ(0.4f, result);
    ASSERT_FLOAT_EQ(7.f, point.x);
    ASSERT_FLOAT_EQ(5.f, point.y);
}

TEST(ProjectPointOntoLineTest, RightTest)
{
    sf::Vector2f point(7.f, 2.f);

    auto result = ECSE::projectPointOntoLine(point,
                                             sf::Vector2f(0.f, 0.f),
                                             sf::Vector2f(5.f, 0.f));

    ASSERT_FLOAT_EQ(1.4f, result);
    ASSERT_FLOAT_EQ(7.f, point.x);
    ASSERT_FLOAT_EQ(0.f, point.y);
}

TEST(ProjectPointOntoLineTest, LeftTest)
{
    sf::Vector2f point(-2.f, 2.f);

    auto result = ECSE::projectPointOntoLine(point,
                                             sf::Vector2f(0.f, 0.f),
                                             sf::Vector2f(5.f, 0.f));

    ASSERT_FLOAT_EQ(-0.4f, result);
    ASSERT_FLOAT_EQ(-2.f, point.x);
    ASSERT_FLOAT_EQ(0.f, point.y);
}

TEST(ProjectPointOnLineTest, AngleTest)
{
    sf::Vector2f point(1.f, 4.f);

    auto result = ECSE::projectPointOntoLine(point,
                                             sf::Vector2f(0.f, 0.f),
                                             sf::Vector2f(5.f, 5.f));

    ASSERT_FLOAT_EQ(0.5f, result);
    ASSERT_FLOAT_EQ(2.5f, point.x);
    ASSERT_FLOAT_EQ(2.5f, point.y);
}

TEST(CircleCircleTest, CollideTest)
{
    float collisionTime;
    sf::Vector2f normal;
    
    ECSE::circleCircle(sf::Vector2f(0.f, 0.f), 5.f,
                       sf::Vector2f(15.f, 0.f), 5.f,
                       sf::Vector2f(10.f, 0.f),
                       collisionTime, normal);

    ASSERT_FLOAT_EQ(0.5f, collisionTime);
    ASSERT_FLOAT_EQ(1.f, normal.x);
    ASSERT_FLOAT_EQ(0.f, normal.y);
}

TEST(CircleCircleTest, AlreadyCollidingTest)
{
    float collisionTime;
    sf::Vector2f normal;

   ECSE::circleCircle(sf::Vector2f(0.f, 0.f), 5.f,
                      sf::Vector2f(1.f, 1.f), 4.f,
                      sf::Vector2f(10.f, -6.f),
                      collisionTime, normal);

    ASSERT_FLOAT_EQ(0.f, collisionTime);
    ASSERT_FLOAT_EQ(sqrt(0.5f), normal.x);
    ASSERT_FLOAT_EQ(sqrt(0.5f), normal.y);
}

TEST(CircleCircleTest, PassThroughTest)
{
    float collisionTime;
    sf::Vector2f normal;

    ECSE::circleCircle(sf::Vector2f(25.f, 0.f), 10.f,
                       sf::Vector2f(25.f, 100.f), 10.f,
                       sf::Vector2f(0.f, 200.f),
                       collisionTime, normal);

    ASSERT_FLOAT_EQ(0.4f, collisionTime);
    ASSERT_FLOAT_EQ(0.f, normal.x);
    ASSERT_FLOAT_EQ(1.f, normal.y);
}

// moveDist < betweenDist
TEST(CircleCircleTest, TooSlowTest)
{
    float collisionTime;
    sf::Vector2f normal;

    ECSE::circleCircle(sf::Vector2f(0.f, 0.f), 5.f,
                       sf::Vector2f(15.f, 0.f), 5.f,
                       sf::Vector2f(0.f, 1.f),
                       collisionTime, normal);

    ASSERT_FLOAT_EQ(-1.f, collisionTime);
}

// towardDist <= 0
TEST(CircleCircleTest, OppositeDirectionTest)
{
    float collisionTime;
    sf::Vector2f normal;

    ECSE::circleCircle(sf::Vector2f(0.f, 0.f), 5.f,
                       sf::Vector2f(15.f, 0.f), 5.f,
                       sf::Vector2f(-10.f, 0.f),
                       collisionTime, normal);

    ASSERT_FLOAT_EQ(-1.f, collisionTime);
}

// shortestDistSqr >= sumRadiiSqr
TEST(CircleCircleTest, WrongDirectionTest)
{
    float collisionTime;
    sf::Vector2f normal;

    ECSE::circleCircle(sf::Vector2f(0.f, 0.f), 5.f,
                       sf::Vector2f(15.f, 0.f), 5.f,
                       sf::Vector2f(50.f, 50.f),
                       collisionTime, normal);

    ASSERT_FLOAT_EQ(-1.f, collisionTime);
}

// Regression: make sure normal corresponds to final circle position, not initial
TEST(CircleCircleTest, ObliqueNormalTest)
{
    float collisionTime;
    sf::Vector2f normal;

    ECSE::circleCircle(sf::Vector2f(0.f, 0.f), 5.f,
                       sf::Vector2f(15.f, 0.f), 5.f,
                       sf::Vector2f(20.f, 1.f),
                       collisionTime, normal);

    ASSERT_GT(collisionTime, 0.f);
    ASSERT_GT(normal.x, 0.f);

    // This is the important check -- this should not be 0
    ASSERT_GT(normal.y, 0.f);
}

TEST(CircleLineTest, AlreadyCollidingTest)
{
    float collisionTime;
    sf::Vector2f normal;

    ECSE::circleLine(sf::Vector2f(2.f, 2.f), 3.f,
                     sf::Vector2f(0.f, 0.f), sf::Vector2f(0.f, 5.f),
                     sf::Vector2f(2.f, 5.f),
                     collisionTime, normal);

    ASSERT_FLOAT_EQ(0.f, collisionTime);
    ASSERT_FLOAT_EQ(-1.f, normal.x);
    ASSERT_FLOAT_EQ(0.f, normal.y);
}

TEST(CircleLineTest, VelocityIntersectTest)
{
    float collisionTime;
    sf::Vector2f normal;

    ECSE::circleLine(sf::Vector2f(-5.f, 5.f), 1.f,
                     sf::Vector2f(-5.f, 0.f), sf::Vector2f(5.f, 0.f),
                     sf::Vector2f(10.f, -10.f),
                     collisionTime, normal);

    ASSERT_FLOAT_EQ(0.4f, collisionTime);
    ASSERT_FLOAT_EQ(0.f, normal.x);
    ASSERT_FLOAT_EQ(-1.f, normal.y);
}

TEST(CircleLineTest, FallShortTest)
{
    float collisionTime;
    sf::Vector2f normal;

    ECSE::circleLine(sf::Vector2f(-5.f, 5.f), 1.f,
                     sf::Vector2f(-5.f, 0.f), sf::Vector2f(5.f, 0.f),
                     sf::Vector2f(2.f, -2.f),
                     collisionTime, normal);

    ASSERT_FLOAT_EQ(-1.f, collisionTime);
}

TEST(CircleLineTest, GrazeStartTest)
{
    float collisionTime;
    sf::Vector2f normal;

    ECSE::circleLine(sf::Vector2f(0.f, 0.f), 2.f,
                     sf::Vector2f(1.f, 10.f), sf::Vector2f(11.f, 10.f),
                     sf::Vector2f(0.f, 10.f),
                     collisionTime, normal);

    ASSERT_GT(collisionTime, 0.f);
    ASSERT_GT(normal.x, 0.f);
    ASSERT_GT(normal.y, 0.f);
}

TEST(CircleLineTest, GrazeEndTest)
{
    float collisionTime;
    sf::Vector2f normal;

    ECSE::circleLine(sf::Vector2f(12.f, 0.f), 2.f,
                     sf::Vector2f(1.f, 10.f), sf::Vector2f(11.f, 10.f),
                     sf::Vector2f(0.f, 10.f),
                     collisionTime, normal);

    ASSERT_GT(collisionTime, 0.f);
    ASSERT_LT(normal.x, 0.f);
    ASSERT_GT(normal.y, 0.f);
}

TEST(CircleLineTest, GrazeMiddleTest)
{
    float collisionTime;
    sf::Vector2f normal;

    ECSE::circleLine(sf::Vector2f(0.f, 0.f), 5.f,
                     sf::Vector2f(-20.f, 6.f), sf::Vector2f(20.f, 6.f),
                     sf::Vector2f(10.f, 2.f),
                     collisionTime, normal);

    ASSERT_FLOAT_EQ(collisionTime, 0.5f);
    ASSERT_FLOAT_EQ(normal.x, 0.f);
    ASSERT_FLOAT_EQ(normal.y, 1.f);
}

TEST(CircleLineTest, FarGrazeMiddleTest)
{
    float collisionTime;
    sf::Vector2f normal;

    ECSE::circleLine(sf::Vector2f(-40.f, -8.f), 5.f,
                     sf::Vector2f(-20.f, 6.f), sf::Vector2f(20.f, 6.f),
                     sf::Vector2f(50.f, 10.f),
                     collisionTime, normal);

    ASSERT_FLOAT_EQ(collisionTime, 0.9f);
    ASSERT_FLOAT_EQ(normal.x, 0.f);
    ASSERT_FLOAT_EQ(normal.y, 1.f);
}

// Only start point is hit
TEST(CircleLineTest, ParallelStartTest)
{
    float collisionTime;
    sf::Vector2f normal;

    ECSE::circleLine(sf::Vector2f(0.f, 0.f), 3.f,
                     sf::Vector2f(1.f, 10.f), sf::Vector2f(1.f, 20.f),
                     sf::Vector2f(0.f, 10.f),
                     collisionTime, normal);

    ASSERT_GT(collisionTime, 0.f);
    ASSERT_GT(normal.x, 0.f);
    ASSERT_GT(normal.y, 0.f);
}

// Only end point is hit
TEST(CircleLineTest, ParallelEndTest)
{
    float collisionTime;
    sf::Vector2f normal;

    ECSE::circleLine(sf::Vector2f(0.f, 30.f), 3.f,
                     sf::Vector2f(1.f, 10.f), sf::Vector2f(1.f, 20.f),
                     sf::Vector2f(0.f, -10.f),
                     collisionTime, normal);

    ASSERT_GT(collisionTime, 0.f);
    ASSERT_GT(normal.x, 0.f);
    ASSERT_LT(normal.y, 0.f);
}

// Both endpoints are hit; min is chosen
TEST(CircleLineTest, ParallelMinTest)
{
    float collisionTime;
    sf::Vector2f normal;

    ECSE::circleLine(sf::Vector2f(0.f, 0.f), 3.f,
                     sf::Vector2f(1.f, 10.f), sf::Vector2f(1.f, 20.f),
                     sf::Vector2f(0.f, 30.f),
                     collisionTime, normal);

    ASSERT_GT(collisionTime, 0.f);
    ASSERT_GT(normal.x, 0.f);
    ASSERT_GT(normal.y, 0.f);
}

// Neither point is hit
TEST(CircleLineTest, ParallelMissTest)
{
    float collisionTime;
    sf::Vector2f normal;

    ECSE::circleLine(sf::Vector2f(15.f, 0.f), 3.f,
                     sf::Vector2f(1.f, 10.f), sf::Vector2f(1.f, 20.f),
                     sf::Vector2f(0.f, 10.f),
                     collisionTime, normal);

    ASSERT_EQ(collisionTime, -1.f);
}

// Moving away from the line
TEST(CircleLineTest, MovingAwayTest)
{
    float collisionTime;
    sf::Vector2f normal;

    ECSE::circleLine(sf::Vector2f(5.f, 0.f), 3.f,
                     sf::Vector2f(0.f, 10.f), sf::Vector2f(0.f, -10.f),
                     sf::Vector2f(20.f, 0.f),
                     collisionTime, normal);

    ASSERT_FLOAT_EQ(collisionTime, -1.f);
}