#include "Entity.h"

namespace ECSE
{

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
