#include "Entity.h"

Entity::Entity()
{
}

Entity::~Entity()
{
}

const std::map<size_t, Component*>& Entity::getComponents() const
{
    return components;
}