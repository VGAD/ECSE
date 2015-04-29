#pragma once

#include "Component.h"

//! A Component which stores the Entity's depth.
namespace ECSE
{

class DepthComponent : public Component
{
public:
    int depth = 0;  //!< The depth of the entity. Lower-depth entities are drawn later.
};

}
