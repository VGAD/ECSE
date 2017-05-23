#include "ECSE/World.h"
#include "ECSE/Engine.h"
#include "ECSE/VectorMath.h"
#include "ControllableBall.h"
#include "Bindings.h"

namespace ReplayDemo
{

void ControllableBall::init()
{
    inputMan = &world->getEngine()->inputManager;

    const float radius = 16.f;

    circle.setFillColor(sf::Color(255, 255, 255));
    circle.setRadius(radius);
    circle.setOrigin(sf::Vector2f(radius, radius) * 0.5f);

    transform = entity->getComponent<ECSE::TransformComponent>();
}

void ControllableBall::update(sf::Time deltaTime)
{
    auto movement = sf::Vector2f(
        inputMan->getIntValue(Bindings::MoveX),
        inputMan->getIntValue(Bindings::MoveY)
    );
    ECSE::normalize(movement);
    movement *= moveSpeed * deltaTime.asSeconds();

    transform->setDeltaPosition(movement);
}

void ControllableBall::render(float alpha, sf::RenderTarget &renderTarget)
{
    circle.setPosition(transform->getInterpLocalPosition(alpha));
    renderTarget.draw(circle);
}

}