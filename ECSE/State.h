#pragma once

#include <SFML/System.hpp>

//! Handles a single game state.
class State
{
    friend class Engine;

public:
    //! Create the State in the given Engine.
    /*!
    * \param engine The State's engine.
    */
    class State(Engine* engine)
        : engine(engine)
    {
    }

    //! Destroy the State.
    virtual ~State() {};

    //! Called when this becomes active.
    virtual void activate() {};

    //! Called when this becomes inactive (not popped, but not being updated).
    virtual void deactivate() {};

    //! Perform the update step.
    /*!
    * \param deltaTime The time elapsed in this update.
    */
    virtual void update(sf::Time deltaTime) = 0;

    //! Perform the advance step.
    virtual void advance() = 0;

    //! Perform the render step.
    /*!
    * \param alpha The amount of interpolation in this render.
    */
    virtual void render(float alpha) = 0;

    //! Get the name of this State class.
    /*!
    * \return The name of the class.
    */  
    virtual const std::string getName() = 0;

    //! Get the Engine to which this belongs.
    /*!
    * \return A pointer to the Engine to which this belongs.
    */
    inline Engine* getEngine() const
    {
        return engine;
    }

protected:
    //! The Engine to which this belongs.
    Engine* engine = nullptr;
};
