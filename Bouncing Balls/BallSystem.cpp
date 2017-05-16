#include "BallSystem.h"
#include "BallComponent.h"

namespace BouncingBalls
{

BallSystem::BallSystem(ECSE::World* world)
    : SetSystem(world), renderTarget(*world->getEngine()->getRenderTarget())
{
    circleShape.setOutlineThickness(1.f);
    circleShape.setFillColor(sf::Color::Transparent);
}

void BallSystem::added()
{
    collisionSystem = world->getSystem<ECSE::CollisionSystem>();

    if (collisionSystem == nullptr)
    {
        throw std::runtime_error("BallSystem requires a CollisionSystem");
    }

    transformSystem = world->getSystem<ECSE::TransformSystem>();

    if (transformSystem == nullptr)
    {
        throw std::runtime_error("BallSystem requires a TransformSystem");
    }
}

void BallSystem::update(sf::Time deltaTime)
{
    SetSystem::update(deltaTime);

    for (auto* e : getEntities())
    {
        // Move balls
        auto ball = e->getComponent<BallComponent>();
        auto lastPos = transformSystem->getGlobalPosition(*e);
        transformSystem->setNextGlobalPosition(*e, lastPos + ball->velocity * deltaTime.asSeconds());
    }
}

void BallSystem::render(float alpha, sf::RenderTarget& renderTarget)
{
    SetSystem::render(alpha, renderTarget);

    for (auto* e : getEntities())
    {
        drawBall(*e);
    }
}

bool BallSystem::checkRequirements(const ECSE::Entity& e) const
{
    if (!e.getComponent<BallComponent>()) return false;

    return true;
}

void BallSystem::drawBall(const ECSE::Entity& e)
{
    auto collider = e.getComponent<ECSE::CircleColliderComponent>();
    if (collider == nullptr) return;

    auto transform = e.getComponent<ECSE::TransformComponent>();
    sf::Vector2f pos = collisionSystem->getColliderPosition(e);

    // Draw the collider
    circleShape.setRadius(collider->radius);
    circleShape.setOrigin(sf::Vector2f(1.f, 1.f) * collider->radius);
    circleShape.setOutlineColor(sf::Color::White);
    circleShape.setPosition(pos);

    renderTarget.draw(circleShape);
}

}