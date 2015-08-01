#pragma once

#include <functional>
#include <unordered_map>
#include <memory>
#include <string>
#include <SFML/Window.hpp>

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

    //! Bind a function to an id.
    /*!
    * \param bindingId The id used to refer to this input source.
    * \param mode The input mode in which this binding is active.
    * \param fn The function to poll the input source's value.
    * \param sensitivity The minimum absolute float value above which the input is returned as non-zero.
    *                    If <= 0, all values are considered non-zero.
    *                    If the input type is non-floating-point, this is ignored.
    */
    template <typename T>
    void bindInput(unsigned bindingId, unsigned mode, const std::function<T()>& fn, float sensitivity = 0.f);

    //! Bind a keyboard key to an id.
    /*!
    * \param bindingId The id used to refer to this input source.
    * \param mode The input mode in which this binding is active.
    * \param key The key value to check.
    */
    void bindInput(unsigned bindingId, unsigned mode, sf::Keyboard::Key key);

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
    void bindInput(unsigned bindingId, unsigned mode, sf::Keyboard::Key key1, sf::Keyboard::Key key2);

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
    void bindInput(unsigned bindingId, unsigned mode, unsigned joystick, sf::Joystick::Axis axis, float sensitivity = 0.f);

    //! Bind a joystick button to an id.
    /*!
    * \param bindingId The id used to refer to this input source.
    * \param mode The input mode in which this binding is active.
    * \param joystick The joystick to check.
    * \param button The button to check.
    */
    void bindInput(unsigned bindingId, unsigned mode, unsigned joystick, unsigned button);

    //! Check if an id is already bound.
    /*!
    * \param bindingId The id to check.
    * \param mode The input mode.
    * \return Whether the id is bound already.
    */
    bool isBound(unsigned bindingId, unsigned mode) const;

    //! Check if an id is already bound in the current input mode.
    /*!
    * \param bindingId The id to check.
    * \return Whether the id is bound already.
    */
    bool isBound(unsigned bindingId) const;

    //! Get an input source's value as a float.
    /*!
    * \param bindingId The id of the input source.
    * \param mode The input mode.
    * \return The input source's value.
    */
    float getFloatValue(unsigned bindingId, unsigned mode) const;

    //! Get an input source's value as a float in the current input mode.
    /*!
    * \param bindingId The id of the input source.
    * \return The input source's value.
    */
    float getFloatValue(unsigned bindingId) const;

    //! Get an input source's value as an int.
    /*!
    * \param bindingId The id of the input source.
    * \param mode The input mode.
    * \return The input source's value.
    */
    int getIntValue(unsigned bindingId, unsigned mode) const;

    //! Get an input source's value as an int in the current input mode.
    /*!
    * \param bindingId The id of the input source.
    * \return The input source's value.
    */
    int getIntValue(unsigned bindingId) const;

    //! Set the input mode.
    /*!
    * \param The new input mode.
    */
    void setInputMode(unsigned mode);

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

private:
    //! A generic class to get data from an input source.
    class InputSource
    {
    public:
        virtual ~InputSource() {}

        //! Get the input source's floating-point value.
        /*!
        * \return The value converted to a float.
        */
        virtual float getFloatValue() const = 0;

        //! Get the input source's integer value.
        /*!
        * \return The value converted to an integer, usually based on sensitivity.
        */
        virtual int getIntValue() const = 0;
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

        inline virtual float getFloatValue() const override {
            float value = fn();

            if (fabs(value) > sensitivity) return value;

            return 0.f;
        }

        inline virtual int getIntValue() const override {
            float value = getFloatValue();

            if (value == 0.f) return 0;
            if (value > 0.f) return 1;

            return -1;
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

        inline virtual float getFloatValue() const override { return static_cast<float>(fn()); }
        inline virtual int getIntValue() const override { return fn(); }
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

        inline virtual float getFloatValue() const override { return static_cast<float>(getIntValue()); }
        inline virtual int getIntValue() const override { return fn() ? 1 : 0; }
    };

    //! Get the input source associated with a binding id.
    /*!
    * \param bindingId The source's id.
    * \param mode The input mode.
    * \return The InputSource.
    */
    const InputSource& getSource(unsigned bindingId, unsigned mode) const;

    //! Determine whether to ignore input.
    /*!
    * \return True if input should be ignored.
    */
    inline bool ignoreInput() const
    {
        return requireFocus && (window == nullptr || !window->hasFocus());
    }

    unsigned inputMode = 0;                 //!< Current input mode.
    const sf::Window* window = nullptr;     //!< The game window, or nullptr if there isn't one.
    bool requireFocus = true;               //!< If true, input is only considered when the window has focus.

    //! Map from mode to bindings, which map from binding id to input source.
    std::unordered_map<unsigned, std::unordered_map<unsigned, std::unique_ptr<InputSource>>> bindings;
};


/////////////////
// Implementation

template <typename T>
void InputManager::bindInput(unsigned bindingId, unsigned mode, const std::function<T()>& fn, float sensitivity)
{
    if (isBound(bindingId, mode))
    {
        throw std::runtime_error("Input id " + std::to_string(bindingId) + " in mode " + std::to_string(mode) + " is already bound");
    }

    auto& modeBindings = bindings[mode];
    modeBindings[bindingId] = std::make_unique<TypedInputSourceImpl<T>>(fn, sensitivity);
}

}