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

sf::Vector2f TransformSystem::getGlobalPosition(const Entity& e) const
{
    auto trans = e.getComponent<TransformComponent>();
    auto pos = trans->getLocalPosition();

    if (trans->parent == Entity::invalidID) return pos;

    auto parentTrans = getParentTransform(*trans);

    return parentTrans->getLocalPosition() + rotate(pos, parentTrans->getLocalAngle());
}

float TransformSystem::getGlobalAngle(const Entity& e) const
{
    auto trans = e.getComponent<TransformComponent>();
    auto angle = trans->getLocalAngle();

    if (trans->parent == Entity::invalidID) return angle;

    auto parentTrans = getParentTransform(*trans);

    return parentTrans->getLocalAngle() + angle;
}

sf::Vector2f TransformSystem::getNextGlobalPosition(const Entity& e) const
{
    auto trans = e.getComponent<TransformComponent>();
    auto pos = trans->getNextLocalPosition();

    if (trans->parent == Entity::invalidID) return pos;

    auto parentTrans = getParentTransform(*trans);

    return parentTrans->getNextLocalPosition() + rotate(pos, parentTrans->getNextLocalAngle());
}

float TransformSystem::getNextGlobalAngle(const Entity& e) const
{
    auto trans = e.getComponent<TransformComponent>();
    auto angle = trans->getNextLocalAngle();

    if (trans->parent == Entity::invalidID) return angle;

    auto parentTrans = getParentTransform(*trans);

    return parentTrans->getNextLocalAngle() + angle;
}

sf::Vector2f TransformSystem::getInterpGlobalPosition(const Entity& e, float alpha) const
{
    auto trans = e.getComponent<TransformComponent>();
    auto pos = trans->getInterpLocalPosition(alpha);

    if (trans->parent == Entity::invalidID) return pos;

    auto parentTrans = getParentTransform(*trans);

    return parentTrans->getInterpLocalPosition(alpha) + rotate(pos, parentTrans->getInterpLocalAngle(alpha));
}

float TransformSystem::getInterpGlobalAngle(const Entity& e, float alpha) const
{
    auto trans = e.getComponent<TransformComponent>();
    auto angle = trans->getInterpLocalAngle(alpha);

    if (trans->parent == Entity::invalidID) return angle;

    auto parentTrans = getParentTransform(*trans);

    return parentTrans->getInterpLocalAngle(alpha) + angle;
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

    auto parentPos = getGlobalPosition(parent);
    auto parentAngle = getGlobalAngle(parent);
    auto childPos = getGlobalPosition(child);
    auto childAngle = getGlobalAngle(child);

    convertRelativeToAnchor(childPos, childAngle, parentPos, parentAngle);

    childTransform->angle = childAngle;
    childTransform->position = childPos;

    auto parentNextPos = getNextGlobalPosition(parent);
    auto parentNextAngle = getNextGlobalAngle(parent);
    auto childNextPos = getNextGlobalPosition(child);
    auto childNextAngle = getNextGlobalAngle(child);

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

    auto childPos = getGlobalPosition(child);
    auto childAngle = getGlobalAngle(child);

    convertRelativeToAnchor(childPos, childAngle, origin, originAngle);

    childTransform->angle = childAngle;
    childTransform->position = childPos;


    auto childNextPos = getNextGlobalPosition(child);
    auto childNextAngle = getNextGlobalAngle(child);

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
