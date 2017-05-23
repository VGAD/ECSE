#include "ECSE/Engine.h"
#include "ECSE/TransformSystem.h"
#include "ECSE/SpecializationSystem.h"
#include "ECSE/SpecializationComponent.h"
#include "ReplayDemo.h"
#include "Bindings.h"
#include "ControllableBall.h"

namespace ReplayDemo
{

ReplayDemoState::ReplayDemoState(ECSE::Engine* engine)
    : WorldState(engine)
{
    world.addSystem<ECSE::SpecializationSystem>();
    world.addSystem<ECSE::TransformSystem>();

    // Set up the ball
    auto ballId = world.createEntity();
    world.attachComponent<ECSE::TransformComponent>(ballId);

    auto specComponent = world.attachComponent<ECSE::SpecializationComponent>(ballId);
    specComponent->setSpecialization(std::make_unique<ControllableBall>());

    world.registerEntity(ballId);
}

void ReplayDemoState::advance()
{
    ECSE::WorldState::advance();


}

}