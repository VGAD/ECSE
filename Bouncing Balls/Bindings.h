//! \file Bindings.h Contains a function to set up input bindings.

#pragma once

#include "ECSE/InputManager.h"

namespace BouncingBalls
{

//! Input binding ids.
enum Bindings
{
    ChangeSpeed
};

//! Possible input modes.
enum InputModes
{
    Keyboard
};

//! Add all the Magnaut input bindings to the given input manager.
/*!
* \param manager The input manager.
*/
extern void bindInputs(ECSE::InputManager& manager);

}