#include "ECSE/Engine.h"
#include "ECSE/TransformSystem.h"
#include "CollisionTestB.h"
#include "TestUtil.h"
#include "CollisionSystem.h"
#include "CollisionDebugSystem.h"

namespace Magnaut {

void CollisionTestStateB::advance()
{
    WorldState::advance();
}

CollisionTestStateB::CollisionTestStateB(ECSE::Engine* engine)
    : WorldState(engine)
{
    world.addSystem<CollisionDebugSystem>();
    world.addSystem<CollisionSystem>();
    world.addSystem<ECSE::TransformSystem>();

    createLine(world, sf::Vector2f(10.f, 10.f), sf::Vector2f(200.f, 100.f), sf::Vector2f(60.f, 60.f));
    createCircle(world, sf::Vector2f(200.f, 50.f), sf::Vector2f(50.f, 150.f), 20.f);
}

}
