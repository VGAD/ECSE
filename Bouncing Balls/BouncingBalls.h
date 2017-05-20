#pragma once

#include "ECSE/WorldState.h"
#include <random>

namespace BouncingBalls
{

//! Tests circle-circle collisions.
class BouncingBallsState : public ECSE::WorldState
{
public:
    //! Construct the state.
    explicit BouncingBallsState(ECSE::Engine* engine);

    //! Advance the state.
    void advance() override;

    //! Get the state's name.
    std::string getName() override { return "BouncingBallsState"; }

private:
    //! Minimum number of balls.
    const unsigned int minBalls = 1;

    //! Maximum number of balls.
    const unsigned int maxBalls = 50;

    //! Minimum ball size.
    const float minBallSize = 5.f;

    //! Maximum ball size.
    const float maxBallSize = 20.f;

    //! The active balls.
    std::vector<ECSE::Entity::ID> balls;

    //! Random number generator.
    std::mt19937 rng;

    //! Create a bouncing ball.
    void createBall(sf::Vector2f start, sf::Vector2f direction, float radius);

    //! Create a random ball.
    void createRandomBall();

    //! Create a wall.
    /*!
    * \param start The line's start position.
    * \param vec The vector describing the line segment.
    */
    void createWall(sf::Vector2f start, sf::Vector2f vec);
};

}