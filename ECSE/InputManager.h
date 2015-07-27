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

    //! Bind a function to an id.
    /*!
    * \param bindingId The id used to refer to this input source.
    * \param mode The input mode in which this binding is active.
    * \param fn The function to poll the input source's value.
    */
    template <typename T>
    void bindInput(unsigned bindingId, unsigned mode, const std::function<T()>& fn);

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
    */
    void bindInput(unsigned bindingId, unsigned mode, unsigned joystick, sf::Joystick::Axis axis);

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
        explicit TypedInputSource(const std::function<T()>& fn)
            : fn(fn)
        { }

    protected:
        std::function<T()> fn;  //!< The function to get the source's value.
    };

    //! Input source with implementation of functions.
    template <typename T>
    class TypedInputSourceImpl
    {
    public:
        explicit TypedInputSourceImpl<T>(const std::function<float()>&)
        {
            throw std::runtime_error("Input type not supported");
        }
    };

    //! Floating-point input source.
    template <>
    class TypedInputSourceImpl<float> : public TypedInputSource<float>
    {
    public:
        explicit TypedInputSourceImpl<float>(const std::function<float()>& fn)
            : TypedInputSource<float>(fn)
        { }

        virtual ~TypedInputSourceImpl<float>() {}

        inline virtual float getFloatValue() const override { return fn(); }
        inline virtual int getIntValue() const override { return static_cast<int>(fn()); }
    };

    //! Integer input source.
    template <>
    class TypedInputSourceImpl<int> : public TypedInputSource<int>
    {
    public:
        explicit TypedInputSourceImpl(const std::function<int()>& fn)
            : TypedInputSource<int>(fn)
        { }

        inline virtual float getFloatValue() const override { return static_cast<float>(fn()); }
        inline virtual int getIntValue() const override { return fn(); }
    };

    //! Boolean input source.
    template <>
    class TypedInputSourceImpl<bool> : public TypedInputSource<bool>
    {
    public:
        explicit TypedInputSourceImpl(const std::function<bool()>& fn)
            : TypedInputSource<bool>(fn)
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

    unsigned inputMode = 0; //!< Current input mode.

    //! Map from mode to bindings, which map from binding id to input source.
    std::unordered_map<unsigned, std::unordered_map<unsigned, std::unique_ptr<InputSource>>> bindings;
};


/////////////////
// Implementation

template <typename T>
void InputManager::bindInput(unsigned bindingId, unsigned mode, const std::function<T()>& fn)
{
    if (isBound(bindingId, mode))
    {
        throw std::runtime_error("Input id " + std::to_string(bindingId) + " in mode " + std::to_string(mode) + " is already bound");
    }

    auto& modeBindings = bindings[mode];
    modeBindings[bindingId] = std::make_unique<TypedInputSourceImpl<T>>(fn);
}

}