#pragma once

#include "ECSE/WorldState.h"

namespace Magnaut
{

//! Tests circle-circle collisions.
class CollisionTestStateB : public ECSE::WorldState
{
public:
    //! Construct the state.
    explicit CollisionTestStateB(ECSE::Engine* engine);

    //! Perform the advance step.
    void advance() override;

    //! Get the state's name.
    std::string getName() override { return "CollisionTestStateB"; }
};

}
