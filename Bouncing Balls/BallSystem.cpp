#include "BallSystem.h"
#include "BallComponent.h"
#include "Bindings.h"

namespace BouncingBalls
{

BallSystem::BallSystem(ECSE::World* world)
    : SetSystem(world), renderTarget(*world->getEngine()->getRenderTarget())
{
    // Set up circle
    circleShape.setOutlineThickness(1.f);
    circleShape.setFillColor(sf::Color::Transparent);

    // Set up text
    font.loadFromFile("C:/Windows/Fonts/arial.ttf");
    text.setFont(font);
    text.setFillColor(sf::Color(255, 255, 255, 255));
    text.setOutlineColor(sf::Color(0, 0, 0, 255));
    text.setOutlineThickness(1.f);
    text.setCharacterSize(16);

    inputMan = &(world->getEngine()->inputManager);
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

    float speed = ballSpeed * deltaTime.asSeconds();

    for (auto* e : getEntities())
    {
        // Move balls
        auto ball = e->getComponent<BallComponent>();
        ball->speed = speed;

        auto lastPos = transformSystem->getGlobalPosition(*e);
        transformSystem->setNextGlobalPosition(*e, lastPos + ball->direction * speed);
    }

    ballSpeed += inputMan->getIntValue(Bindings::ChangeSpeed) * ballSpeedChange;
    if (ballSpeed < 0.f) ballSpeed = 0.f;
}

void BallSystem::advance()
{
    for (auto* e : getEntities())
    {
        auto ball = e->getComponent<BallComponent>();
        ball->hitCount = 0;
        ball->lastHitTime = 0;
    }
}

void BallSystem::render(float alpha, sf::RenderTarget& renderTarget)
{
    SetSystem::render(alpha, renderTarget);

    for (auto* e : getEntities())
    {
        drawBall(*e);
    }

    // Draw help text
    std::stringstream strstr;

    text.setPosition(sf::Vector2f(10.f, 10.f));
    strstr << "Up/down: change speed (currently " << ballSpeed << " pixels/second)";
    text.setString(strstr.str());
    renderTarget.draw(text);

    strstr = std::stringstream();
    text.setPosition(sf::Vector2f(10.f, 30.f));
    strstr << "Left/right: change ball count (currently " << getEntities().size() << ")";
    text.setString(strstr.str());
    renderTarget.draw(text);
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