#include "BallSystem.h"
#include "BallComponent.h"
#include "Bindings.h"

namespace BouncingBalls
{

static const float thickness = 1.f;
static const sf::Color centerColor = sf::Color(0, 0, 0, 0);
static const sf::Color endColor = sf::Color(255, 255, 255);
static const sf::Color trailColor = sf::Color(200, 0, 0);

//! Resize and center a circle.
/*!
* \param circle The circle.
* \param radius The radius of the circle.
*/
static void resizeCircle(sf::CircleShape& circle, float radius)
{
    circle.setRadius(radius);
    circle.setOrigin(sf::Vector2f(1.f, 1.f) * radius);
}

BallSystem::BallSystem(ECSE::World* world)
    : SetSystem(world), renderTarget(*world->getEngine()->getRenderTarget())
{
// Get system dependent font
#if defined(_WIN32)
    font.loadFromFile("C:/Windows/Fonts/arial.ttf");
#elif defined(linux) || defined(__linux)
    font.loadFromFile("/usr/share/fonts/truetype/freefont/FreeSans.ttf");
// Yes this is intended to be &&
#elif defined(_APPLE_) && defined(_MACH_)
    font.loadFromFile("/Library/Fonts/Arial.ttf");
#endif

    // Set up text
    text.setFont(font);
    text.setFillColor(sf::Color(255, 255, 255, 255));
    text.setOutlineColor(sf::Color(0, 0, 0, 255));
    text.setOutlineThickness(1.f);
    text.setCharacterSize(16);

    circleShape.setOutlineThickness(thickness);
    circleShape.setFillColor(centerColor);

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

        ball->trajectory.clear();
        ball->trajectory.push_back(e->getComponent<ECSE::TransformComponent>()->getLocalPosition());
    }
}

void BallSystem::render(float alpha, sf::RenderTarget& renderTarget)
{
    SetSystem::render(alpha, renderTarget);

    // Draw balls
    for (auto e : getEntities())
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

    strstr = std::stringstream();
    text.setPosition(sf::Vector2f(10.f, 50.f));
    strstr << "Enter: toggle ball trails";
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
    auto transform = e.getComponent<ECSE::TransformComponent>();
    auto ball = e.getComponent<BallComponent>();

    float radius = collider->radius;
    sf::Vector2f start = collisionSystem->getColliderPosition(e);

    // Set up circle
    resizeCircle(circleShape, collider->radius);

    // Draw trail
    if (drawTrails && ball->trajectory.size() > 0)
    {
        circleShape.setOutlineColor(trailColor);

        auto lastPos = ball->trajectory[0];
        for (auto pos : ball->trajectory)
        {
            circleShape.setPosition(pos);

            renderTarget.draw(circleShape);
            drawLine(lastPos, pos, trailColor);

            lastPos = pos;
        }

        // Draw trail line connecting to current position
        drawLine(lastPos, start, trailColor);
    }

    // Draw start position, which is where it ended up after the last advance step
    circleShape.setOutlineColor(endColor);
    circleShape.setPosition(start);
    renderTarget.draw(circleShape);
}

void BallSystem::drawLine(const sf::Vector2f& start, const sf::Vector2f& end,
                                const sf::Color& color)
{
    sf::Vertex line[] = {
        sf::Vertex(start, color),
        sf::Vertex(end, color)
    };
    renderTarget.draw(line, 2, sf::Lines);
}

}
