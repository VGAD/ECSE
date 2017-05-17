#include "Bindings.h"

namespace BouncingBalls
{

void bindInputs(ECSE::InputManager& manager)
{
    manager.bindInput(ChangeSpeed, Keyboard, sf::Keyboard::Down, sf::Keyboard::Up);
}

}