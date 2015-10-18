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

void TransformSystem::markToRemove(Entity& e)
{
    auto trans = e.getComponent<TransformComponent>();
    for (auto childId : trans->getChildren())
    {
        auto child = world->getEntity(childId);
        if (child == nullptr)
        {
            throw std::runtime_error("Child with id #" + std::to_string(childId) + " does not exist");
        }

        auto childTrans = child->getComponent<TransformComponent>();
        if (childTrans->destroyWithParent)
        {
            world->destroyEntity(childId);
        }
        else
        {
            unparentEntity(*child);
        }
    }

    SetSystem::markToRemove(e);
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

void TransformSystem::setNextGlobalPosition(const Entity& e, sf::Vector2f newPosition, bool discrete) const
{
    auto trans = e.getComponent<TransformComponent>();

    if (trans->parent == Entity::invalidID)
    {
        trans->setNextLocalPosition(newPosition, discrete);
        return;
    }

    auto parent = world->getEntity(trans->parent);

    auto parentPos = getNextGlobalPosition(*parent);
    auto parentAngle = getNextGlobalAngle(*parent);

    convertPositionRelativeToAnchor(newPosition, parentPos, parentAngle);

    trans->setNextLocalPosition(newPosition, discrete);
}

void TransformSystem::setNextGlobalAngle(const Entity& e, float newAngle, bool discrete) const
{
    auto trans = e.getComponent<TransformComponent>();

    if (trans->parent == Entity::invalidID)
    {
        trans->setNextLocalAngle(newAngle, discrete);
        return;
    }

    auto parent = world->getEntity(trans->parent);

    auto parentAngle = getNextGlobalAngle(*parent);

    convertAngleRelativeToAnchor(newAngle, parentAngle);

    trans->setNextLocalAngle(newAngle, discrete);
}

void TransformSystem::setGlobalPosition(const Entity& e, sf::Vector2f newPosition, bool setNext) const
{
    auto trans = e.getComponent<TransformComponent>();

    if (trans->parent == Entity::invalidID)
    {
        trans->setLocalPosition(newPosition, setNext);
        return;
    }

    auto parent = world->getEntity(trans->parent);

    auto parentPos = getGlobalPosition(*parent);
    auto parentAngle = getGlobalAngle(*parent);

    convertPositionRelativeToAnchor(newPosition, parentPos, parentAngle);

    trans->setLocalPosition(newPosition, setNext);
}

void TransformSystem::setGlobalAngle(const Entity& e, float newAngle, bool setNext) const
{
    auto trans = e.getComponent<TransformComponent>();

    if (trans->parent == Entity::invalidID)
    {
        trans->setLocalAngle(newAngle, setNext);
        return;
    }

    auto parent = world->getEntity(trans->parent);

    auto parentAngle = getGlobalAngle(*parent);

    convertAngleRelativeToAnchor(newAngle, parentAngle);

    trans->setLocalAngle(newAngle, setNext);
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

    convertAngleRelativeToAnchor(childAngle, parentAngle);
    convertPositionRelativeToAnchor(childPos, parentPos, parentAngle);

    childTransform->angle = childAngle;
    childTransform->position = childPos;

    auto parentNextPos = getNextGlobalPosition(parent);
    auto parentNextAngle = getNextGlobalAngle(parent);
    auto childNextPos = getNextGlobalPosition(child);
    auto childNextAngle = getNextGlobalAngle(child);

    convertAngleRelativeToAnchor(childNextAngle, parentNextAngle);
    convertPositionRelativeToAnchor(childNextPos, parentNextPos, parentNextAngle);

    childTransform->nextAngle = childNextAngle;
    childTransform->nextPosition = childNextPos;


    childTransform->parent = parent.getID();
    parentTransform->children.push_back(child.getID());
}

void TransformSystem::unparentEntity(const Entity& child) const
{
    auto childTransform = child.getComponent<TransformComponent>();
    auto childId = child.getID();

    if (childTransform == nullptr)
    {
        throw std::runtime_error("Child entity (#" + std::to_string(childId) + ") does not have a transform component");
    }

    if (childTransform->parent == Entity::invalidID)
    {
        throw std::runtime_error("Tried to unparent parentless entity (#" + std::to_string(childId) + ")");
    }

    auto parent = world->getEntity(childTransform->parent);
    if (parent == nullptr)
    {
        throw std::runtime_error("Parent entity (#" + std::to_string(childTransform->parent) + ") does not exist");
    }

    auto origin = sf::Vector2f(0.f, 0.f);
    auto originAngle = 0.f;

    auto childPos = getGlobalPosition(child);
    auto childAngle = getGlobalAngle(child);

    convertAngleRelativeToAnchor(childAngle, originAngle);
    convertPositionRelativeToAnchor(childPos, origin, originAngle);

    childTransform->angle = childAngle;
    childTransform->position = childPos;


    auto childNextPos = getNextGlobalPosition(child);
    auto childNextAngle = getNextGlobalAngle(child);

    convertAngleRelativeToAnchor(childNextAngle, originAngle);
    convertPositionRelativeToAnchor(childNextPos, origin, originAngle);

    childTransform->nextAngle = childNextAngle;
    childTransform->nextPosition = childNextPos;

    childTransform->parent = Entity::invalidID;


    auto parentTransform = parent->getComponent<TransformComponent>();
    auto& parentChildren = parentTransform->children;
    auto it = std::find(parentChildren.begin(), parentChildren.end(), childId);

    if (it == parentChildren.end())
    {
        throw std::runtime_error("Parent entity (#" + std::to_string(childTransform->parent) + ") does not have child #" + std::to_string(childId));
    }

    parentChildren.erase(it);
}

void TransformSystem::convertAngleRelativeToAnchor(float& angle, float anchorAngle)
{
    angle -= anchorAngle;
}

void TransformSystem::convertPositionRelativeToAnchor(sf::Vector2f& pos, const sf::Vector2f& anchorPos, float anchorAngle)
{
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
