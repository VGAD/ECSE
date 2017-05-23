#pragma once

#include "ECSE/WorldState.h"

namespace ReplayDemo
{

//! Demonstrates replay functionality.
class ReplayDemoState : public ECSE::WorldState
{
public:
    //! Construct the state.
    explicit ReplayDemoState(ECSE::Engine* engine);

    //! Advance state.
    void advance() override;

    //! Get the state's name.
    std::string getName() override { return "ReplayDemoState"; }
};

}