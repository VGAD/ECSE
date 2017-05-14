#pragma once

#include "ECSE/WorldState.h"

namespace Magnaut
{

//! Tests circle-circle collisions.
class CollisionTestStateA : public ECSE::WorldState
{
public:
    //! Construct the state.
    explicit CollisionTestStateA(ECSE::Engine* engine);

    //! Perform the advance step.
    void advance() override;

    //! Get the state's name.
    std::string getName() override { return "CollisionTestStateA"; }

private:
    //! Create a debug circle and add it to the world.
    /*!
    * The circle will flash potential further collisions assuming it can continue past the first hit.
    *
    * \param start The circle's start position.
    * \param end The circle's next position.
    * \param radius The radius of the circle.
    * \param discrete Whether this should be a discrete jump.
    * \return The Entity ID of the created circle.
    */
    ECSE::Entity::ID createFlashingCircle(sf::Vector2f start, sf::Vector2f end, float radius, bool discrete = false);
};

}
