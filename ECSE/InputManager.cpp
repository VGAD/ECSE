#include "InputManager.h"

namespace ECSE
{

InputManager::InputManager()
{
}

void InputManager::bindInput(unsigned bindingId, unsigned mode, sf::Keyboard::Key key)
{
    std::function<bool()> poll = [key]() { return sf::Keyboard::isKeyPressed(key); };
    bindInput(bindingId, mode, poll);
}

void InputManager::bindInput(unsigned bindingId, unsigned mode, sf::Keyboard::Key key1, sf::Keyboard::Key key2)
{
    std::function<int()> poll = [key1, key2]()
    {
        auto pull = sf::Keyboard::isKeyPressed(key1) ? 1 : 0;
        auto push = sf::Keyboard::isKeyPressed(key2) ? 1 : 0;

        return push - pull;
    };
    bindInput(bindingId, mode, poll);
}


void InputManager::bindInput(unsigned bindingId, unsigned mode, unsigned joystick, sf::Joystick::Axis axis, float sensitivity)
{
    std::function<float()> poll = [joystick, axis]()
    {
        return sf::Joystick::getAxisPosition(joystick, axis) / 100.f;
    };
    bindInput(bindingId, mode, poll, sensitivity);
}


void InputManager::bindInput(unsigned bindingId, unsigned mode, unsigned joystick, unsigned button)
{
    std::function<int()> poll = [joystick, button]()
    {
        return sf::Joystick::isButtonPressed(joystick, button);
    };
    bindInput(bindingId, mode, poll);
}

bool InputManager::isBound(unsigned bindingId, unsigned mode) const
{
    auto modeBindings = bindings.find(mode);
    if (modeBindings == bindings.end()) return false;

    auto source = modeBindings->second.find(bindingId);
    if (source == modeBindings->second.end()) return false;

    return true;
}

bool InputManager::isBound(unsigned bindingId) const
{
    return isBound(bindingId, inputMode);
}

float InputManager::getFloatValue(unsigned bindingId, unsigned mode) const
{
    const InputSource& source = getSource(bindingId, mode);

    return source.getFloatValue();
}

float InputManager::getFloatValue(unsigned bindingId) const
{
    return getFloatValue(bindingId, inputMode);
}

int InputManager::getIntValue(unsigned bindingId, unsigned mode) const
{
    const InputSource& source = getSource(bindingId, mode);

    return source.getIntValue();
}

int InputManager::getIntValue(unsigned bindingId) const
{
    return getIntValue(bindingId, inputMode);
}

void InputManager::setInputMode(unsigned mode)
{
    inputMode = mode;
}

unsigned InputManager::getInputMode() const
{
    return inputMode;
}

const InputManager::InputSource& InputManager::getSource(unsigned bindingId, unsigned mode) const
{
    auto modeBindings = bindings.find(mode);

    if (modeBindings == bindings.end())
    {
        throw std::runtime_error("No bindings for input mode " + std::to_string(mode));
    }

    auto source = modeBindings->second.find(bindingId);

    if (source == modeBindings->second.end())
    {
        throw std::runtime_error("No source bound to id " + std::to_string(bindingId) + " in input mode " + std::to_string(mode));
    }

    return *source->second;
}

}
