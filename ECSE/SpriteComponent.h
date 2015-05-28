#pragma once

#include "Component.h"
#include "Spritemap.h"

namespace ECSE
{

//! A Component which stores Sprite data.
class SpriteComponent : public Component
{
public:
    Spritemap sprite;   //!< The animated sprite.
};

}
