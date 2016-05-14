#include "InputManager.h"
#include "Logging.h"

namespace ECSE
{

InputManager::InputManager()
{
}

void InputManager::update()
{
    sf::Vector2i newMousePosition;
    if (monkeyMode)
    {
        newMousePosition = sf::Vector2i(rand(), rand());
    }
    else if (window != nullptr && !(demoMouse && playingDemo))
    {
        newMousePosition = sf::Mouse::getPosition(*window);
    }

    if (playingDemo)
    {
        for (auto& modePair : demoSources)
        {
            for (auto& bindingPair : modePair.second)
            {
                auto& source = bindingPair.second;

                source->updatePrevInternalValue();
            }
        }

        // Time to read the next change frame
        if (nextChangeFrame < demoFrames || nextChangeFrame == 0)
        {
            uint8_t delta;
            demoIn->read(reinterpret_cast<char*>(&delta), sizeof(delta));

            // Full time will be specified
            if (delta == maxDelta)
            {
                demoIn->read(reinterpret_cast<char*>(&nextChangeFrame), sizeof(nextChangeFrame));
            }
            else
            {
                nextChangeFrame = demoFrames + delta;
            }
        }
        
        if (nextChangeFrame == demoFrames)
        {
            readDemoChanges();

            if (demoIn->peek() == EOF)
            {
                stopDemo();
            }
        }
    }
    else
    {
        auto changes = std::set<std::pair<uint8_t, uint8_t>>();

        for (auto& modePair : bindings)
        {
            for (auto& bindingPair : modePair.second)
            {
                auto& source = bindingPair.second;
                source->updatePrevInternalValue();

                if (monkeyMode)
                {
                    source->setInternalValue(rand());
                }
                else
                {
                    source->updateInternalValue();
                }

                if (source->getInternalValue() != source->getPrevInternalValue() || demoFrames == 0)
                {
                    changes.insert(std::pair<uint8_t, uint8_t>(modePair.first, bindingPair.first));
                }
            }
        }

        // Write out recording data
        if (recording &&
            (!changes.empty() ||
            prevInputMode != inputMode ||
            (newMousePosition != mousePosition && demoMouse)))
        {
            writeDemoChanges(changes);
        }
        mousePosition = newMousePosition;
    }

    if (playingDemo || recording)
    {
        ++demoFrames;
    }

    prevInputMode = inputMode;
}

void InputManager::bindInput(uint8_t bindingId, uint8_t mode, sf::Keyboard::Key key)
{
    std::function<bool()> poll = [key]() { return sf::Keyboard::isKeyPressed(key); };
    bindInput(bindingId, mode, poll);
}

void InputManager::bindInput(uint8_t bindingId, uint8_t mode, sf::Keyboard::Key key1, sf::Keyboard::Key key2)
{
    std::function<int8_t()> poll = [key1, key2]()
    {
        auto pull = sf::Keyboard::isKeyPressed(key1) ? 1 : 0;
        auto push = sf::Keyboard::isKeyPressed(key2) ? 1 : 0;

        return push - pull;
    };
    bindInput(bindingId, mode, poll);
}

void InputManager::bindInput(uint8_t bindingId, uint8_t mode, unsigned joystick, sf::Joystick::Axis axis, float sensitivity)
{
    std::function<float()> poll = [joystick, axis]()
    {
        return sf::Joystick::getAxisPosition(joystick, axis) / 100.f;
    };
    bindInput(bindingId, mode, poll, sensitivity);
}

void InputManager::bindInput(uint8_t bindingId, uint8_t mode, unsigned joystick, unsigned button)
{
    std::function<bool()> poll = [joystick, button]()
    {
        return sf::Joystick::isButtonPressed(joystick, button);
    };
    bindInput(bindingId, mode, poll);
}

bool InputManager::isBound(uint8_t bindingId, uint8_t mode) const
{
    auto modeBindings = bindings.find(mode);
    if (modeBindings == bindings.end()) return false;

    auto source = modeBindings->second.find(bindingId);
    if (source == modeBindings->second.end()) return false;

    return true;
}

bool InputManager::isBound(uint8_t bindingId) const
{
    return isBound(bindingId, inputMode);
}

float InputManager::getFloatValue(uint8_t bindingId, uint8_t mode) const
{
    if (ignoreInput()) return 0.f;

    const InputSource& source = getSource(bindingId, mode);

    return source.getFloatValue();
}

float InputManager::getFloatValue(uint8_t bindingId) const
{
    return getFloatValue(bindingId, inputMode);
}

float InputManager::getFloatDelta(uint8_t bindingId, uint8_t mode) const
{
    if (ignoreInput()) return 0.f;

    const InputSource& source = getSource(bindingId, mode);

    return source.getFloatValue() - source.getPrevFloatValue();
}

float InputManager::getFloatDelta(uint8_t bindingId) const
{
    return getFloatDelta(bindingId, inputMode);
}

int8_t InputManager::getIntValue(uint8_t bindingId, uint8_t mode) const
{
    if (ignoreInput()) return 0;

    const InputSource& source = getSource(bindingId, mode);

    return source.getIntValue();
}

int8_t InputManager::getIntValue(uint8_t bindingId) const
{
    return getIntValue(bindingId, inputMode);
}

int8_t InputManager::getIntDelta(uint8_t bindingId, uint8_t mode) const
{
    if (ignoreInput()) return 0;

    const InputSource& source = getSource(bindingId, mode);

    return source.getIntValue() - source.getPrevIntValue();
}

int8_t InputManager::getIntDelta(uint8_t bindingId) const
{
    return getIntDelta(bindingId, inputMode);
}

sf::Vector2i InputManager::getMousePosition() const
{
    return mousePosition;
}

bool InputManager::setInputMode(uint8_t mode)
{
    if (playingDemo) return false;

    inputMode = mode;
    return true;
}

unsigned InputManager::getInputMode() const
{
    return inputMode;
}

std::vector<unsigned> InputManager::getConnectedJoysticks() const
{
    std::vector<unsigned> result;

    for (unsigned i = 0; i < sf::Joystick::Count; ++i)
    {
        if (sf::Joystick::isConnected(i)) result.push_back(i);
    }

    return result;
}

void InputManager::startRecording(std::ostream& stream, bool recordMousePos)
{
    if (playingDemo)
    {
        throw std::runtime_error("Can't record and play a demo at the same time");
    }

    if (recording)
    {
        stopRecording();
    }

    demoOut = &stream;
    recording = true;
    demoMouse = recordMousePos;
    demoFrames = lastChangeFrame = 0;

    demoOut->write(reinterpret_cast<char*>(&demoMouse), sizeof(demoMouse));

    // Write out previous values so delta is correct
    auto changes = std::set<std::pair<uint8_t, uint8_t>>();
    for (auto& modePair : bindings)
    {
        for (auto& bindingPair : modePair.second)
        {
            changes.insert(std::pair<uint8_t, uint8_t>(modePair.first, bindingPair.first));
        }
    }
    writeDemoChanges(changes);
}

void InputManager::stopRecording()
{
    if (recording)
    {
        recording = false;

        // Write empty changes if input hasn't changed recently so the demo doesn't end early
        if (lastChangeFrame != demoFrames - 1)
        {
            writeDemoChanges(std::set<std::pair<uint8_t, uint8_t>>());
        }
    }
}

void InputManager::playDemo(std::istream& stream)
{
    if (recording)
    {
        throw std::runtime_error("Can't record and play a demo at the same time");
    }

    if (playingDemo)
    {
        stopDemo();
    }

    demoIn = &stream;
    playingDemo = true;
    demoFrames = lastChangeFrame = nextChangeFrame = 0;

    demoSources.clear();

    demoIn->read(reinterpret_cast<char*>(&demoMouse), sizeof(demoMouse));
    
    // Read changes once so demo sources are primed with previous values
    demoIn->ignore(1); // Skip delta
    readDemoChanges();
}

void InputManager::stopDemo()
{
    playingDemo = false;

    for (auto& modePair : demoSources)
    {
        for (auto& bindingPair : modePair.second)
        {
            auto& demoSource = bindingPair.second;

            getSource(bindingPair.first, modePair.first).setPrevInternalValue(demoSource->getInternalValue());
        }
    }
}

void InputManager::startMonkeyMode()
{
    monkeyMode = true;
    LOG(TRACE) << "Monkey mode enabled";
}

void InputManager::stopMonkeyMode()
{
    monkeyMode = false;
    LOG(TRACE) << "Monkey mode disabled";
}

InputManager::InputSource& InputManager::getSource(uint8_t bindingId, uint8_t mode) const
{
    auto& bindingMap = playingDemo ? demoSources : bindings;
    auto modeBindings = bindingMap.find(mode);

    if (modeBindings == bindingMap.end())
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

void InputManager::writeDemoChanges(const std::set<std::pair<uint8_t, uint8_t>>& changes)
{
    // This is kind of crappy, but hitting the limit would be very difficult
    if (changes.size() >= std::numeric_limits<uint8_t>::max())
    {
        throw std::runtime_error("More than " + std::to_string(std::numeric_limits<uint8_t>::max()) + " input changes per frame not supported");
    }

    if (demoOut == nullptr || !demoOut->good())
    {
        throw std::runtime_error("Demo output stream cannot be written to");
    }


    // Subtract 1 because we'll already be on the next frame when this is read
    uint32_t delta = demoFrames == 0 ? 0 : demoFrames - lastChangeFrame - 1;

    // More than one byte worth of delta time, so use extra bytes to represent exactly
    if (delta > maxDelta)
    {
        uint8_t maxDeltaCopy = maxDelta;
        demoOut->write(reinterpret_cast<char*>(&maxDeltaCopy), sizeof(maxDeltaCopy));

        demoOut->write(reinterpret_cast<char*>(&demoFrames), sizeof(demoFrames));
    }
    // Otherwise just write the delta time as one byte
    else
    {
        uint8_t deltaByte = static_cast<uint8_t>(delta);
        demoOut->write(reinterpret_cast<char*>(&deltaByte), sizeof(deltaByte));
    }

    if (demoMouse)
    {
        demoOut->write(reinterpret_cast<char*>(&mousePosition), sizeof(mousePosition));
    }

    demoOut->write(reinterpret_cast<char*>(&inputMode), sizeof(inputMode));

    auto changeCount = static_cast<uint8_t>(changes.size());
    demoOut->write(reinterpret_cast<char*>(&changeCount), sizeof(changeCount));

    for (auto& pair : changes)
    {
        auto mode = pair.first;
        auto bindingId = pair.second;
        auto value = getSource(bindingId, mode).getInternalValue();

        demoOut->write(reinterpret_cast<char*>(&mode), sizeof(mode));
        demoOut->write(reinterpret_cast<char*>(&bindingId), sizeof(bindingId));
        demoOut->write(reinterpret_cast<char*>(&value), sizeof(value));
    }

    lastChangeFrame = demoFrames;
}

void InputManager::readDemoChanges()
{
    if (demoMouse)
    {
        demoIn->read(reinterpret_cast<char*>(&mousePosition), sizeof(mousePosition));
    }

    demoIn->read(reinterpret_cast<char*>(&inputMode), sizeof(inputMode));

    uint8_t changeCount;
    demoIn->read(reinterpret_cast<char*>(&changeCount), sizeof(changeCount));

    // Update the internal values of any inputs that changed
    for (uint8_t i = 0; i < changeCount; ++i)
    {
        uint8_t mode;
        uint8_t bindingId;
        ECSE_INPUT_INTERNAL_TYPE value;

        demoIn->read(reinterpret_cast<char*>(&mode), sizeof(mode));
        demoIn->read(reinterpret_cast<char*>(&bindingId), sizeof(bindingId));
        demoIn->read(reinterpret_cast<char*>(&value), sizeof(value));

        auto& bindings = demoSources[mode];
        auto sourceIt = bindings.find(bindingId);
        InputSource* source;

        if (sourceIt == bindings.end())
        {
            bindings[bindingId] = std::make_unique<ManualInputSource>();
            source = bindings[bindingId].get();
        }
        else
        {
            source = sourceIt->second.get();
        }

        source->setInternalValue(value);
    }
}

}
