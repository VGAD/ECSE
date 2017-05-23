#include "Bindings.h"

namespace ReplayDemo
{

void bindInputs(ECSE::InputManager& manager)
{
    manager.bindInput(MoveX, Keyboard, sf::Keyboard::Left, sf::Keyboard::Right);
    manager.bindInput(MoveY, Keyboard, sf::Keyboard::Up, sf::Keyboard::Down);
    manager.bindInput(Advance, Keyboard, sf::Keyboard::Space);
}

}