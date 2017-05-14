#pragma once

#include "ColliderComponent.h"

namespace ECSE
{

//! A Component which stores circular collision mask data and collision history.
class CircleColliderComponent : public ColliderComponent
{
public:
    //! This is an extension of ColliderComponent.
    using ExtendsComponent = ColliderComponent;

    float radius;   //!< The circle's radius.
};

}
