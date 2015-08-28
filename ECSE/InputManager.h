#pragma once

#include <functional>
#include <unordered_map>
#include <memory>
#include <string>
#include <ostream>
#include <set>
#include <utility>
#include <SFML/Window.hpp>

// This can be switched out for a larger type to improve input precision, but this will increase the size of replays.
// This type should always be a signed integer type.
#ifndef ECSE_INPUT_INTERNAL_TYPE
#define ECSE_INPUT_INTERNAL_TYPE int8_t
#endif

// The lower this is, the more precise input will be, but the larger replays will be.
// The input will be rounded into "chunks" of size 1 << ECSE_INPUT_PRECISION.
#ifndef ECSE_INPUT_PRECISION
#define ECSE_INPUT_PRECISION 5
#endif

namespace ECSE
{

//! Aggregates input from various sources through a single interface.
class InputManager
{
public:
    //! Construct the input manager.
    explicit InputManager();

    //! Set the game window.
    /*!
    * If the game window is not set, certain features will not work.
    *
    * \param window The game window.
    */
    inline void setWindow(const sf::Window* window)
    {
        this->window = window;
    }

    //! Change whether the input requires window focus.
    /*!
    * \param value If true, input will be ignore unless the window has focus.
    *              If false, input will always be considered.
    *              Note: if no window has been set and this is true, input will be ignored.
    */
    inline void setRequireFocus(bool value)
    {
        this->requireFocus = value;
    }

    //! Update the values of the inputs.
    void update();

    //! Bind a function to an id.
    /*!
    * \param bindingId The id used to refer to this input source.
    * \param mode The input mode in which this binding is active.
    * \param fn The function to poll the input source's value. This should always return values in [-1, 1].
    * \param sensitivity The minimum absolute float value above which the input is returned as non-zero.
    *                    If <= 0, all values are considered non-zero.
    *                    If the input type is non-floating-point, this is ignored.
    */
    template <typename T>
    void bindInput(uint8_t bindingId, uint8_t mode, const std::function<T()>& fn, float sensitivity = 0.f);

    //! Bind a keyboard key to an id.
    /*!
    * \param bindingId The id used to refer to this input source.
    * \param mode The input mode in which this binding is active.
    * \param key The key value to check.
    */
    void bindInput(uint8_t bindingId, uint8_t mode, sf::Keyboard::Key key);

    //! Bind a pair of keys to an id.
    /*!
    * The keys "push and pull" the value as if they are two directions in an axis.
    * key1 will decrease the value by 1 and key2 will increase the value by 1, e.g.
    * both pressed or neither pressed returns 0, only key1 pressed returns -1, and
    * only key2 pressed returns 1.
    *
    * \param bindingId The id used to refer to this input source.
    * \param mode The input mode in which this binding is active.
    * \param key1 The first key value to check.
    * \param key2 The second key value to check.
    */
    void bindInput(uint8_t bindingId, uint8_t mode, sf::Keyboard::Key key1, sf::Keyboard::Key key2);

    //! Bind a joystick axis to an id (scaled to [-1 .. 1] instead of [-100 .. 100]).
    /*!
    * \param bindingId The id used to refer to this input source.
    * \param mode The input mode in which this binding is active.
    * \param joystick The joystick to check.
    * \param axis The axis to check.
    * \param sensitivity The minimum absolute float value above which the input is returned as non-zero.
    *                    If <= 0, all values are considered non-zero.
    *                    If the input type is non-floating-point, this is ignored.
    */
    void bindInput(uint8_t bindingId, uint8_t mode, unsigned joystick, sf::Joystick::Axis axis, float sensitivity = 0.f);

    //! Bind a joystick button to an id.
    /*!
    * \param bindingId The id used to refer to this input source.
    * \param mode The input mode in which this binding is active.
    * \param joystick The joystick to check.
    * \param button The button to check.
    */
    void bindInput(uint8_t bindingId, uint8_t mode, unsigned joystick, unsigned button);

    //! Check if an id is already bound.
    /*!
    * \param bindingId The id to check.
    * \param mode The input mode.
    * \return Whether the id is bound already.
    */
    bool isBound(uint8_t bindingId, uint8_t mode) const;

    //! Check if an id is already bound in the current input mode.
    /*!
    * \param bindingId The id to check.
    * \return Whether the id is bound already.
    */
    bool isBound(uint8_t bindingId) const;

    //! Get an input source's value as a float.
    /*!
    * \param bindingId The id of the input source.
    * \param mode The input mode.
    * \return The input source's value.
    */
    float getFloatValue(uint8_t bindingId, uint8_t mode) const;

    //! Get an input source's value as a float in the current input mode.
    /*!
    * \param bindingId The id of the input source.
    * \return The input source's value.
    */
    float getFloatValue(uint8_t bindingId) const;

    //! Get an input source's value as an int.
    /*!
    * \param bindingId The id of the input source.
    * \param mode The input mode.
    * \return The input source's value.
    */
    int getIntValue(uint8_t bindingId, uint8_t mode) const;

    //! Get an input source's value as an int in the current input mode.
    /*!
    * \param bindingId The id of the input source.
    * \return The input source's value.
    */
    int getIntValue(uint8_t bindingId) const;

    //! Set the input mode.
    /*!
    * \param mode The new input mode.
    */
    void setInputMode(uint8_t mode);

    //! Get the current input mode.
    /*!
    * \return The current input mode.
    */
    unsigned getInputMode() const;

    //! Get a vector of all available joystick ids.
    /*!
    * \return A vector containing the ids of the available joysticks.
    */
    std::vector<unsigned> getConnectedJoysticks() const;

    //! Start recording a demo.
    /*!
    * \param stream The object to which demo data will be streamed.
    */
    void startRecording(std::ostream& stream);

    //! Stop recording the demo.
    void stopRecording();

    //! Start playing back a demo.
    /*!
    * \param stream The object from which demo data will be streamed.
    */
    void playDemo(std::istream& stream);

    //! Stop playing back the demo.
    void stopDemo();

    //! Check if this is recording a demo.
    /*!
    * \return True if this is recording a demo.
    */
    inline bool isRecording() { return recording; }

    //! Check if this is playing a demo.
    /*!
    * \return True if this is playing a demo.
    */
    inline bool isPlayingDemo() { return playingDemo; }

private:
    //! A generic class to get data from an input source.
    class InputSource
    {
    public:
        //! Half of the number of possible values for the internal value.
        const ECSE_INPUT_INTERNAL_TYPE halfValue = static_cast<size_t>((1 << sizeof(ECSE_INPUT_INTERNAL_TYPE) * 7) - 1);

        //! Input should be rounded into "chunks" of this size.
        const ECSE_INPUT_INTERNAL_TYPE precision = 1 << ECSE_INPUT_PRECISION;

        //! Destroy the input source.
        virtual ~InputSource() {}

        //! Update the input source's internal value.
        /*!
        * \return The value converted to an integer for external storage.
        */
        virtual void updateInternalValue() = 0;

        //! Get the input source's internal value.
        /*!
        * \return The value converted to an integer for external storage.
        */
        inline ECSE_INPUT_INTERNAL_TYPE getInternalValue() const { return internalVal; }

        //! Get the input source's floating-point value.
        /*!
        * \return The value converted to a float.
        */
        inline float getFloatValue() const
        {
            auto value = getInternalValue();

            if (value == 0) return 0.f;

            return static_cast<float>(value) / static_cast<float>(halfValue);
        }

        //! Get the input source's integer value.
        /*!
        * \return The value converted to an integer.
        */
        inline int getIntValue() const
        {
            auto value = getInternalValue();

            if (value == 0) return 0;
            if (value > 0) return 1;

            return -1;
        }

    protected:
        ECSE_INPUT_INTERNAL_TYPE internalVal;   //!< The internal value.
    };

    //! Templated InputSource subclass that holds the actual polling function.
    template <typename T>
    class TypedInputSource : public InputSource
    {
    public:
        //! Construct the TypedInputSource.
        /*!
        * \param fn The function to poll the input source's value.
        */
        explicit TypedInputSource(const std::function<T()>& fn, float sensitivity)
            : fn(fn), sensitivity(sensitivity)
        { }

    protected:
        std::function<T()> fn;  //!< The function to get the source's value.
        float sensitivity;      //!< The sensitivity level above which a non-zero value is reported.
    };

    //! Input source with implementation of functions.
    template <typename T>
    class TypedInputSourceImpl
    {
    public:
        explicit TypedInputSourceImpl<T>(const std::function<float()>&, float)
        {
            throw std::runtime_error("Input type not supported");
        }
    };

    //! Floating-point input source.
    template <>
    class TypedInputSourceImpl<float> : public TypedInputSource<float>
    {
    public:
        explicit TypedInputSourceImpl<float>(const std::function<float()>& fn, float sensitivity)
            : TypedInputSource<float>(fn, sensitivity)
        { }

        virtual ~TypedInputSourceImpl<float>() {}

        inline virtual void updateInternalValue() override
        {
            float newValue = fn();

            if (fabs(newValue) < sensitivity)
            {
                internalVal = 0;
                return;
            }

            auto reduced = static_cast<ECSE_INPUT_INTERNAL_TYPE>(newValue * halfValue);
            if (abs(reduced) != halfValue)
            {
                reduced = (reduced / precision) * precision;
            }

            internalVal = reduced;
        }
    };

    //! Integer input source.
    template <>
    class TypedInputSourceImpl<int> : public TypedInputSource<int>
    {
    public:
        explicit TypedInputSourceImpl(const std::function<int()>& fn, float sensitivity)
            : TypedInputSource<int>(fn, sensitivity)
        { }

        inline virtual void updateInternalValue() override
        {
            auto newValue = fn();

            if (newValue == 0)
            {
                internalVal = 0;
            }
            else if (newValue > 0)
            {
                internalVal = halfValue;
            }
            else
            {
                internalVal = -halfValue;
            }
        }
    };

    //! Boolean input source.
    template <>
    class TypedInputSourceImpl<bool> : public TypedInputSource<bool>
    {
    public:
        explicit TypedInputSourceImpl(const std::function<bool()>& fn, float sensitivity)
            : TypedInputSource<bool>(fn, sensitivity)
        {
        }

        inline virtual void updateInternalValue() override { internalVal = fn() ? halfValue : 0; }
    };

    //! Input source which just has an internal value that can be updated by another class.
    class InternalInputSource : public InputSource
    {
    public:
        inline virtual void updateInternalValue() override { }

        //! Set the internal value.
        /*!
        * \param value The new value.
        */
        inline void setInternalValue(ECSE_INPUT_INTERNAL_TYPE value) { internalVal = value; }
    };

    //! Get the input source associated with a binding id.
    /*!
    * \param bindingId The source's id.
    * \param mode The input mode.
    * \return The InputSource.
    */
    const InputSource& getSource(uint8_t bindingId, uint8_t mode) const;

    //! Write out data for changes in the current frame to the demo stream.
    void writeChanges(const std::set<std::pair<uint8_t, uint8_t>>& changes);

    //! Determine whether to ignore input.
    /*!
    * \return True if input should be ignored.
    */
    inline bool ignoreInput() const
    {
        return requireFocus && (window == nullptr || !window->hasFocus());
    }

    uint8_t inputMode = 0;                  //!< Current input mode.
    const sf::Window* window = nullptr;     //!< The game window, or nullptr if there isn't one.
    bool requireFocus = true;               //!< If true, input is only considered when the window has focus.
    bool recording = false;                 //!< Whether a demo is being recorded.
    bool playingDemo = false;               //!< Whether a demo is being played.
    bool firstRecordedFrame = false;        //!< Whether this is the first frame to be recorded.

    std::ostream* demoOut = nullptr;        //!< Output stream for the demo.
    std::istream* demoIn = nullptr;         //!< Input stream for the demo.

    //! Map from mode to bindings, which map from binding id to input source.
    std::unordered_map<uint8_t, std::unordered_map<uint8_t, std::unique_ptr<InputSource>>> bindings;

    //! Map from mode to demo sources, which map from binding id to input source.
    std::unordered_map<uint8_t, std::unordered_map<uint8_t, std::unique_ptr<InputSource>>> demoSources;
};


/////////////////
// Implementation

template <typename T>
void InputManager::bindInput(uint8_t bindingId, uint8_t mode, const std::function<T()>& fn, float sensitivity)
{
    if (isBound(bindingId, mode))
    {
        throw std::runtime_error("Input id " + std::to_string(bindingId) + " in mode " + std::to_string(mode) + " is already bound");
    }

    auto& modeBindings = bindings[mode];
    modeBindings[bindingId] = std::make_unique<TypedInputSourceImpl<T>>(fn, sensitivity);
}

}