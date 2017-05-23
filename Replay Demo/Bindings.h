//! \file Bindings.h Contains a function to set up input bindings.

#pragma once

#include "ECSE/InputManager.h"

namespace ReplayDemo
{

//! Input binding ids.
enum Bindings
{
    MoveX,
    MoveY,
    Advance
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