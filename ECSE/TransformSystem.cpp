#include "TransformSystem.h"
#include "VectorMath.h"
#include "World.h"

namespace ECSE
{

void TransformSystem::advance()
{
    SetSystem::advance();

    for (Entity* entity : getEntities())
    {
        TransformComponent* tc = entity->getComponent<TransformComponent>();
        if (!tc->enabled) continue;

        tc->advance();
    }
}

bool TransformSystem::checkRequirements(const Entity& e) const
{
    if (!e.getComponent<TransformComponent>()) return false;

    return true;
}

sf::Vector2f TransformSystem::getPosition(const Entity& e) const
{
    auto trans = e.getComponent<TransformComponent>();
    auto pos = trans->getPosition();

    if (trans->parent == Entity::invalidID) return pos;

    auto parentTrans = getParentTransform(*trans);

    return parentTrans->getPosition() + rotate(pos, parentTrans->getAngle());
}

float TransformSystem::getAngle(const Entity& e) const
{
    auto trans = e.getComponent<TransformComponent>();
    auto angle = trans->getAngle();

    if (trans->parent == Entity::invalidID) return angle;

    auto parentTrans = getParentTransform(*trans);

    return parentTrans->getAngle() + angle;
}

sf::Vector2f TransformSystem::getNextPosition(const Entity& e) const
{
    auto trans = e.getComponent<TransformComponent>();
    auto pos = trans->getNextPosition();

    if (trans->parent == Entity::invalidID) return pos;

    auto parentTrans = getParentTransform(*trans);

    return parentTrans->getNextPosition() + rotate(pos, parentTrans->getNextAngle());
}

float TransformSystem::getNextAngle(const Entity& e) const
{
    auto trans = e.getComponent<TransformComponent>();
    auto angle = trans->getNextAngle();

    if (trans->parent == Entity::invalidID) return angle;

    auto parentTrans = getParentTransform(*trans);

    return parentTrans->getNextAngle() + angle;
}

sf::Vector2f TransformSystem::interpolatePosition(const Entity& e, float alpha) const
{
    auto trans = e.getComponent<TransformComponent>();
    auto pos = trans->interpolatePosition(alpha);

    if (trans->parent == Entity::invalidID) return pos;

    auto parentTrans = getParentTransform(*trans);

    return parentTrans->interpolatePosition(alpha) + rotate(pos, parentTrans->interpolateAngle(alpha));
}

float TransformSystem::interpolateAngle(const Entity& e, float alpha) const
{
    auto trans = e.getComponent<TransformComponent>();
    auto angle = trans->interpolateAngle(alpha);

    if (trans->parent == Entity::invalidID) return angle;

    auto parentTrans = getParentTransform(*trans);

    return parentTrans->interpolateAngle(alpha) + angle;
}

void TransformSystem::parentEntity(const Entity& child, const Entity& parent) const
{
    auto childTransform = child.getComponent<TransformComponent>();
    auto parentTransform = parent.getComponent<TransformComponent>();

    if (childTransform == nullptr)
    {
        throw std::runtime_error("Child entity (#" + std::to_string(child.getID()) + ") does not have a transform component");
    }

    if (parentTransform == nullptr)
    {
        throw std::runtime_error("Parent entity (#" + std::to_string(parent.getID()) + ") does not have a transform component");
    }

    // Remove old parent first
    if (childTransform->parent != Entity::invalidID)
    {
        unparentEntity(child);
    }

    auto parentPos = getPosition(parent);
    auto parentAngle = getAngle(parent);
    auto childPos = getPosition(child);
    auto childAngle = getAngle(child);

    convertRelativeToAnchor(childPos, childAngle, parentPos, parentAngle);

    childTransform->angle = childAngle;
    childTransform->position = childPos;

    auto parentNextPos = getNextPosition(parent);
    auto parentNextAngle = getNextAngle(parent);
    auto childNextPos = getNextPosition(child);
    auto childNextAngle = getNextAngle(child);

    convertRelativeToAnchor(childNextPos, childNextAngle, parentNextPos, parentNextAngle);

    childTransform->nextAngle = childNextAngle;
    childTransform->nextPosition = childNextPos;

    childTransform->parent = parent.getID();
}

void TransformSystem::unparentEntity(const Entity& child) const
{
    auto childTransform = child.getComponent<TransformComponent>();

    if (childTransform == nullptr)
    {
        throw std::runtime_error("Child entity (#" + std::to_string(child.getID()) + ") does not have a transform component");
    }

    if (childTransform->parent == Entity::invalidID)
    {
        throw std::runtime_error("Tried to unparent parentless entity (#" + std::to_string(child.getID()) + ")");
    }

    auto origin = sf::Vector2f(0.f, 0.f);
    auto originAngle = 0.f;

    auto childPos = getPosition(child);
    auto childAngle = getAngle(child);

    convertRelativeToAnchor(childPos, childAngle, origin, originAngle);

    childTransform->angle = childAngle;
    childTransform->position = childPos;


    auto childNextPos = getNextPosition(child);
    auto childNextAngle = getNextAngle(child);

    convertRelativeToAnchor(childNextPos, childNextAngle, origin, originAngle);

    childTransform->nextAngle = childNextAngle;
    childTransform->nextPosition = childNextPos;


    childTransform->parent = Entity::invalidID;
}

void TransformSystem::convertRelativeToAnchor(sf::Vector2f& pos, float& angle, sf::Vector2f anchorPos, float anchorAngle)
{
    angle -= anchorAngle;

    auto offset = pos - anchorPos;

    auto xAxis = sf::Vector2f(1.f, 0.f);
    rotate(xAxis, anchorAngle);

    auto yAxis = xAxis;
    rotate90(yAxis);

    pos.x = getDotProduct(offset, xAxis);
    pos.y = getDotProduct(offset, yAxis);
}

TransformComponent* TransformSystem::getParentTransform(const TransformComponent& trans) const
{
    return world->getEntity(trans.parent)->getComponent<TransformComponent>();
}

}
