#pragma once

#include <stack>
#include <queue>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "State.h"
#include "Common.h"

//! Contains the base game information and procedures, such as the game loop and resource management.
class Engine
{
public:
    //! Create the game window and initialize the Engine.
    /*!
    * \param name The window name.
    * \param fps The target FPS.
    */
    Engine(std::string name = "", unsigned int fps = 60);

    //! Destroy all game objects and stop the game.
    ~Engine();

    //! Run the game loop. This will not return until the game is finished running.
    void run();

    //! Pop the top State from the stack.
    /*!
    * The State stack will not be updated until the beginning of the next game loop iteration.
    */
    void popState();

    //! Push a State onto the stack.
    /*!
    * The State stack will not be updated until the beginning of the next game loop iteration.
    *
    * \param state The State to push onto the stack.
    */
    void pushState(std::unique_ptr<State> state);

    //! Get the window's RenderTarget.
    /*!
    * \return The RenderTarget.
    */
    inline sf::RenderTarget* getRenderTarget() const
    {
        return static_cast<sf::RenderTarget*>(window.get());
    }

private:
    ////////////
    // Functions

    //! Poll events and handle them.
    void pollEvents();

    //! Update the State stack.
    /*!
    * \return A reference to the State on top of the stack.
    */
    State& updateStateStack();

    //! Get the current active State.
    /*!
    * \return A reference to the State on top of the stac.
    */
    State& getActiveState() const;

    ///////
    // Data

    //! A stack of States.
    typedef std::stack<std::unique_ptr<State>> StateStack;

    //! Used to track operations on the State stack.
    class StackOperation
    {
    public:
        virtual void execute(Engine* engine) = 0;
    };

    //! Pops the first State from the stack.
    class Pop : public StackOperation
    {
    public:
        void execute(Engine* engine);
    };

    //! Pushes a State onto the stack.
    class Push : public StackOperation
    {
    public:
        Push(std::unique_ptr<State> state);
        void execute(Engine* engine);

    private:
        std::unique_ptr<State> state;
    };

    friend StackOperation;

    StateStack states;                                  //!< The stack of game States. The one at the top is updated in the run loop.
    std::queue<std::unique_ptr<StackOperation>> ops;    //!< The operations to perform on the State stack at the beginning of the next iteration.
    std::unique_ptr<sf::RenderWindow> window;           //!< The display window.
    sf::Time deltaTime;                                 //!< The amount of time per sim update.
};