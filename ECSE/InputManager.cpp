#include "InputManager.h"
#include "Logging.h"

namespace ECSE
{

InputManager::InputManager()
{
}

void InputManager::update()
{
    if (playingDemo)
    {
        uint8_t newInputMode;
        demoIn->read(reinterpret_cast<char*>(&newInputMode), sizeof(inputMode));
        if (newInputMode != inputMode) setInputMode(newInputMode);

        uint8_t changeCount;
        demoIn->read(reinterpret_cast<char*>(&changeCount), sizeof(changeCount));

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

        if (demoIn->peek() == EOF)
        {
            stopDemo();
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
                ECSE_INPUT_INTERNAL_TYPE oldValue = source->getInternalValue();

                if (monkeyMode)
                {
                    source->setInternalValue(rand());
                }
                else
                {
                    source->updateInternalValue();
                }

                if (source->getInternalValue() != oldValue || firstRecordedFrame)
                {
                    changes.insert(std::pair<uint8_t, uint8_t>(modePair.first, bindingPair.first));
                }
            }
        }

        // Write out recording data
        if (recording)
        {
            writeChanges(changes);

            firstRecordedFrame = false;
        }
    }
}

void InputManager::bindInput(uint8_t bindingId, uint8_t mode, sf::Keyboard::Key key)
{
    std::function<bool()> poll = [key]() { return sf::Keyboard::isKeyPressed(key); };
    bindInput(bindingId, mode, poll);
}

void InputManager::bindInput(uint8_t bindingId, uint8_t mode, sf::Keyboard::Key key1, sf::Keyboard::Key key2)
{
    std::function<int()> poll = [key1, key2]()
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
    std::function<int()> poll = [joystick, button]()
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

int InputManager::getIntValue(uint8_t bindingId, uint8_t mode) const
{
    if (ignoreInput()) return 0;

    const InputSource& source = getSource(bindingId, mode);

    return source.getIntValue();
}

int InputManager::getIntValue(uint8_t bindingId) const
{
    return getIntValue(bindingId, inputMode);
}

void InputManager::setInputMode(uint8_t mode)
{
    inputMode = mode;
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

void InputManager::startRecording(std::ostream& stream)
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
    firstRecordedFrame = true;
}

void InputManager::stopRecording()
{
    recording = false;
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

    demoSources.clear();
}

void InputManager::stopDemo()
{
    playingDemo = false;
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

const InputManager::InputSource& InputManager::getSource(uint8_t bindingId, uint8_t mode) const
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

void InputManager::writeChanges(const std::set<std::pair<uint8_t, uint8_t>>& changes)
{
    if (demoOut == nullptr || !demoOut->good())
    {
        throw std::runtime_error("Demo output stream cannot be written to");
    }

    // This is kind of crappy, but hitting the limit would be very difficult
    if (changes.size() >= std::numeric_limits<uint8_t>::max())
    {
        throw std::runtime_error("More than " + std::to_string(std::numeric_limits<uint8_t>::max()) + " input changes per frame not supported");
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
}

}
