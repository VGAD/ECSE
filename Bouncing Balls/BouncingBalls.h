#pragma once

#include "ECSE/WorldState.h"

namespace BouncingBalls
{

//! Tests circle-circle collisions.
class BouncingBallsState : public ECSE::WorldState
{
public:
    //! Construct the state.
    explicit BouncingBallsState(ECSE::Engine* engine);

    //! Get the state's name.
    std::string getName() override { return "BouncingBallsState"; }
};

}