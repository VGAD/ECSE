#pragma once

#include "Component.h"
#include "Spritemap.h"

//! A Component which stores Sprite data.
namespace ECSE
{

class SpriteComponent : public Component
{
public:
    Spritemap sprite;   //!< The animated sprite.
};

}
