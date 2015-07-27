#include "InputManager.h"

namespace ECSE
{

InputManager::InputManager()
{
}

bool InputManager::isBound(unsigned bindingId, unsigned mode) const
{
    auto modeBindings = bindings.find(mode);
    if (modeBindings == bindings.end()) return false;

    auto source = modeBindings->second.find(bindingId);
    if (source == modeBindings->second.end()) return false;

    return true;
}

float InputManager::getFloatValue(unsigned bindingId, unsigned mode) const
{
    const InputSource& source = getSource(bindingId, mode);

    return source.getFloatValue();
}

int InputManager::getIntValue(unsigned bindingId, unsigned mode) const
{
    const InputSource& source = getSource(bindingId, mode);

    return source.getIntValue();
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
