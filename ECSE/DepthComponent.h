#pragma once

#include "Component.h"

namespace ECSE
{

//! A Component which stores the Entity's depth.
class DepthComponent : public Component
{
public:
    int depth = 0;  //!< The depth of the entity. Lower-depth entities are drawn later.
};

}
