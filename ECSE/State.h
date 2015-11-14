#pragma once

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <functional>

namespace ECSE
{

//! Handles a single game state.
class State
{
    friend class Engine;

public:
    /*!
    * Side routines allow you to have a function called continuously at the end of update().
    * The function will keep being called once per update as long as it returns false; when
    * it returns true, it will be removed from the side routine list.
    *
    * Side routines are similar to the idea of coroutines, but named differently to avoid
    * confusion with a true coroutine which can arbitrarily pause execution.
    */
    typedef std::function<bool()> SideRoutine;

    //! Create the State in the given Engine.
    /*!
    * \param engine The State's engine.
    */
    explicit State(Engine* engine)
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
    virtual void update(sf::Time deltaTime);

    //! Perform the advance step.
    virtual void advance() = 0;

    //! Perform the render step.
    /*!
    * \param alpha The amount of interpolation in this render.
    * \param renderTarget The RenderTarget to draw to.
    */
    virtual void render(float alpha, sf::RenderTarget& renderTarget) = 0;

    //! Get the name of this State class.
    /*!
    * \return The name of the class.
    */  
    virtual std::string getName() = 0;

    //! Start a side routine.
    /*!
    * \param fn The function to run.
    */
    virtual void startSideRoutine(SideRoutine fn);

    //! Get the Engine to which this belongs.
    /*!
    * \return A pointer to the Engine to which this belongs.
    */
    inline Engine* getEngine() const
    {
        return engine;
    }

protected:
    Engine* engine = nullptr;               //! The Engine to which this belongs.
    std::vector<SideRoutine> sideRoutines;  //! List of currently running side routines.
};

}
