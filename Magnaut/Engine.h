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
    Engine();

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

private:
    ////////////
    // Functions

    //! Check whether the window should be closed.
    bool checkClose() const;

    //! Update the state stack.
    void updateStateStack();

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
};