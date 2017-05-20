#include "World.h"
#include "VectorMath.h"
#include "CollisionDebugSystem.h"
#include "CircleColliderComponent.h"
#include "LineColliderComponent.h"

namespace ECSE
{

static const float thickness = 1.f;
static const sf::Color clearColor = sf::Color(0, 0, 0, 0);
static const sf::Color startColor = sf::Color(0, 255, 0);
static const sf::Color sweepColor = sf::Color(255, 255, 0);
static const sf::Color discreteJumpColor = sf::Color(255, 0, 255);
static const sf::Color collisionColor = sf::Color(0, 255, 255);
static const sf::Color endColor = sf::Color(255, 0, 0);

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

CollisionDebugSystem::CollisionDebugSystem(World* world)
    : SetSystem(world), renderTarget(*world->getEngine()->getRenderTarget())
{
    circleShape.setOutlineThickness(thickness);
    circleShape.setFillColor(clearColor);
}

void CollisionDebugSystem::advance()
{
    SetSystem::advance();

    collisionBuffer.clear();
}

void CollisionDebugSystem::render(float alpha, sf::RenderTarget& renderTarget)
{
    SetSystem::render(alpha, renderTarget);

    for (Entity* e : getEntities())
    {
        drawCircleSweep(*e);
        drawLineSweep(*e);
    }

    if (drawCollisions)
    {
        // Draw each collision
        for (auto& collision : collisionBuffer)
        {
            drawCollision(collision);
        }
    }
}

bool CollisionDebugSystem::checkRequirements(const Entity& e) const
{
    if (!e.getComponent<TransformComponent>()) return false;

    // Need one of these
    if (e.getComponent<CircleColliderComponent>()) return true;
    if (e.getComponent<LineColliderComponent>()) return true;

    return false;
}

void CollisionDebugSystem::added()
{
    collisionSystem = world->getSystem<CollisionSystem>();

    if (collisionSystem == nullptr)
    {
        throw std::runtime_error("CollisionDebugSystem requires a CollisionSystem");
    }

    if (world->getSystemPosition<CollisionSystem>() < world->getSystemPosition<CollisionDebugSystem>())
    {
        throw std::runtime_error("CollisionDebugSystem should be added before CollisionSystem");
    }
}

void CollisionDebugSystem::internalAddEntity(Entity& e)
{
    SetSystem::internalAddEntity(e);

    // Attach collision listener
    auto listener = std::bind(&CollisionDebugSystem::collisionCallback, this, std::placeholders::_1);
    auto circle = e.getComponent<CircleColliderComponent>();
    auto line = e.getComponent<LineColliderComponent>();

    if (circle) circle->addCallback(listener);
    if (line) line->addCallback(listener);
}

void CollisionDebugSystem::drawCircleSweep(const Entity& e)
{
    auto collider = e.getComponent<CircleColliderComponent>();
    if (collider == nullptr) return;

    float radius = collider->radius;
    auto transform = e.getComponent<TransformComponent>();
    sf::Vector2f start = collisionSystem->getColliderPosition(e);

    // Draw initial position
    resizeCircle(circleShape, collider->radius);
    circleShape.setOutlineColor(startColor);
    circleShape.setPosition(start);
    renderTarget.draw(circleShape);

    if (drawTrails)
    {
        sf::Vector2f end = collisionSystem->getNextColliderPosition(e);

        auto forward = start - end;
        forward = normalize(forward);

        auto up = rotate90(forward) * radius;

        // Draw discrete jump line
        if (transform->isPositionDiscrete())
        {
            sf::Vertex line[] = {
                sf::Vertex(start, discreteJumpColor),
                sf::Vertex(end, discreteJumpColor)
            };
            renderTarget.draw(line, 2, sf::Lines);
        }
        // Draw sweep area
        else
        {
            drawLine(start + up, end + up, sweepColor);
            drawLine(start - up, end - up, sweepColor);
        }

        // Draw final position
        circleShape.setOutlineColor(endColor);
        circleShape.setPosition(end);
        renderTarget.draw(circleShape);
    }
}

void CollisionDebugSystem::drawLineSweep(const Entity& e)
{
    auto collider = e.getComponent<LineColliderComponent>();
    if (collider == nullptr) return;

    auto transform = e.getComponent<TransformComponent>();
    sf::Vector2f start = collisionSystem->getColliderPosition(e);

    sf::Vector2f aa = start;
    sf::Vector2f ba = start + collider->vec;

    // Draw initial position
    drawLine(aa, ba, startColor);

    if (drawTrails)
    {
        sf::Vector2f end = collisionSystem->getNextColliderPosition(e);
        sf::Vector2f ab = end;
        sf::Vector2f bb = end + collider->vec;

        // Draw discrete jump line
        if (transform->isPositionDiscrete())
        {
            drawLine(midpoint(aa, ba),
                     midpoint(ab, bb),
                     discreteJumpColor);
        }
        // Draw sweep area
        else
        {
            drawLine(aa, ab, sweepColor);
            drawLine(ba, bb, sweepColor);
        }

        // Draw final position
        drawLine(ab, bb, endColor);
    }
}

void CollisionDebugSystem::drawCollision(const Collision& collision)
{
    Entity* entity = collision.self;
    auto ts = world->getSystem<TransformSystem>();
    auto collider = collision.self->getComponent<ColliderComponent>();

    // Collider may have been destroyed before we got here
    if (collider == nullptr) return;

    auto colliderOffset = collider->offset;
    rotate(colliderOffset, ts->getInterpGlobalAngle(*collision.self, collision.time));

    auto colliderPos = collision.position + colliderOffset;

    auto circle = entity->getComponent<CircleColliderComponent>();
    if (circle)
    {
        resizeCircle(circleShape, circle->radius);
        circleShape.setOutlineColor(collisionColor);
        circleShape.setPosition(colliderPos);
        renderTarget.draw(circleShape);
    }

    auto line = entity->getComponent<LineColliderComponent>();
    if (line)
    {
        drawLine(colliderPos, colliderPos + line->vec, collisionColor);
    }
}

void CollisionDebugSystem::drawLine(const sf::Vector2f& start, const sf::Vector2f& end,
                                    const sf::Color& color)
{
    sf::Vertex line[] = {
        sf::Vertex(start, color),
        sf::Vertex(end, color)
    };
    renderTarget.draw(line, 2, sf::Lines);
}

ColliderComponent::ChangeSet CollisionDebugSystem::collisionCallback(const Collision& collision)
{
    collisionBuffer.push_back(collision);

    if (verbose)
    {
        LOG(DEBUG) << "Entity #"
                   << collision.self->getID()
                   << " collided at ("
                   << collision.position.x
                   << ", "
                   << collision.position.y
                   << ")";
    }

    return {};
}

}
