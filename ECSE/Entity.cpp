#include "Entity.h"

namespace ECSE
{

const Entity::ID Entity::invalidID = 0;

Entity::Entity()
{
}

Entity::~Entity()
{
}

const boost::unordered_map<size_t, Component*>& Entity::getComponents() const
{
    return components;
}

}
