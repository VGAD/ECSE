#include "ECSE/World.h"
#include "ECSE/VectorMath.h"
#include "ECSE/LineColliderComponent.h"
#include "LineRenderSystem.h"

namespace BouncingBalls
{

static const float thickness = 1.f;
static const sf::Color color = sf::Color(255, 255, 255);

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

LineRenderSystem::LineRenderSystem(ECSE::World* world)
    : SetSystem(world), renderTarget(*world->getEngine()->getRenderTarget())
{

}

void LineRenderSystem::render(float alpha, sf::RenderTarget& renderTarget)
{
    SetSystem::render(alpha, renderTarget);

    for (ECSE::Entity* e : getEntities())
    {
        drawLineEntity(*e);
    }
}

bool LineRenderSystem::checkRequirements(const ECSE::Entity& e) const
{
    if (!e.getComponent<ECSE::TransformComponent>()) return false;

    // Need one of these
    if (e.getComponent<ECSE::LineColliderComponent>()) return true;

    return false;
}

void LineRenderSystem::added()
{
    collisionSystem = world->getSystem<ECSE::CollisionSystem>();

    if (collisionSystem == nullptr)
    {
        throw std::runtime_error("LineRenderSystem requires a CollisionSystem");
    }
}

void LineRenderSystem::drawLineEntity(const ECSE::Entity& e)
{
    auto collider = e.getComponent<ECSE::LineColliderComponent>();
    if (collider == nullptr) return;

    auto transform = e.getComponent<ECSE::TransformComponent>();
    sf::Vector2f start = collisionSystem->getColliderPosition(e);

    sf::Vector2f aa = start;
    sf::Vector2f ba = start + collider->vec;

    // Draw initial position
    drawLine(aa, ba, color);
}

void LineRenderSystem::drawLine(const sf::Vector2f& start, const sf::Vector2f& end,
                                    const sf::Color& color)
{
    sf::Vertex line[] = {
        sf::Vertex(start, color),
        sf::Vertex(end, color)
    };
    renderTarget.draw(line, 2, sf::Lines);
}

}
