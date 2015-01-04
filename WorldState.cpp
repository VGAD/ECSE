#include "WorldState.h"

WorldState::WorldState()
    : world(this)
{
}

WorldState::~WorldState()
{
}

const std::string WorldState::getName()
{
    return "WorldState";
}

void WorldState::update(sf::Time deltaTime)
{
    world.update(deltaTime);
}

void WorldState::advance()
{
    world.advance();
}

void WorldState::render(float alpha)
{
    world.render(alpha);
}