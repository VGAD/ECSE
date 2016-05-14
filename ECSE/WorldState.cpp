#include "WorldState.h"

namespace ECSE
{

WorldState::WorldState(Engine* engine)
    : State(engine), world(this)
{
}

WorldState::~WorldState()
{
}

std::string WorldState::getName()
{
    return "WorldState";
}

void WorldState::update(sf::Time deltaTime)
{
    world.update(deltaTime);
    State::update(deltaTime);
}

void WorldState::advance()
{
    world.advance();
}

void WorldState::render(float alpha, sf::RenderTarget& renderTarget)
{
    world.render(alpha, renderTarget);
}

}
