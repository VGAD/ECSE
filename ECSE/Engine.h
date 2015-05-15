#pragma once

#include <stack>
#include <queue>
#include <memory>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System/Vector2.hpp>
#include "State.h"
#include "ResourceManager.h"
#include "AnimationSet.h"

//! Contains the base game information and procedures, such as the game loop and resource management.
namespace ECSE
{

class Engine
{
public:
    ////////////
    // Functions

    //! Create the game window and initialize the Engine.
    /*!
    * \param size The window dimensions.
    * \param name The window name.
    * \param fps The target FPS.
    */
    explicit Engine(sf::Vector2i size, std::string name = "", unsigned int fps = 60);

    //! Destroy all game objects, clean up resources and stop the game.
    ~Engine();

    //! Run the game loop. This will not return until the game is finished running.
    void run();

    //! Save a screenshot.
    void saveScreenshot();

    //! Pop the top State from the stack.
    /*!
    * The State stack will not be updated until the beginning of the next game loop iteration.
    */
    void popState();

    //! Get the window's RenderTarget.
    /*!
    * \return The RenderTarget.
    */
    inline sf::RenderTarget* getRenderTarget()
    {
        return &renderTarget;
    }

    //! Set the resolution scale.
    /*!
    * \param scale The new scale.
    */
    inline void setScale(float scale)
    {
        rtSprite.setScale(scale, scale);
    }

    //! Get the resolution scale.
    /*!
    * \return The scale.
    */
    inline float getScale() const
    {
        return rtSprite.getScale().x;
    }

    //! Get the number of advances since this started.
    inline size_t getTicks() const
    {
        return ticks;
    }

    //! Exit the game.
    inline void exit()
    {
        toExit = true;
    }

    //! Push a State onto the stack.
    /*!
    * The State stack will not be updated until the beginning of the next game loop iteration.
    *
    * \return A pointer to the State.
    */
    template <typename StateType>
    StateType* pushState();

    //! Get the current active State.
    /*!
    * \return A reference to the State on top of the stack.
    */
    State& getActiveState() const;


    ///////
    // Data
    ResourceManager<sf::Texture> textureManager;    //!< Keeps track of loaded Textures.
    ResourceManager<sf::SoundBuffer> soundManager;  //!< Keeps track of loaded SoundBuffers.
    ResourceManager<AnimationSet> animationManager; //!< Keeps track of loaded AnimationSets.

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


    ///////
    // Data

    //! Maximum amount of time to be simulated before rendering.
    /*!
    * This helps to avoid a spiral of doom when simulation is expensive.
    */
    sf::Time maxElapsed = sf::seconds(0.25f);

    //! Offscreen buffer used to update the window display.
    sf::RenderTexture renderTarget;

    //! Sprite which is used to update the window display.
    sf::Sprite rtSprite;

    //! Number of advances since the engine was started.
    size_t ticks = 0;

    //! A stack of States.
    typedef std::stack<std::unique_ptr<State>> StateStack;

    //! Used to track operations on the State stack.
    class StackOperation
    {
    public:
        virtual ~StackOperation() {};
        virtual void execute(Engine* engine) = 0;
    };

    //! Pops the first State from the stack.
    class Pop : public StackOperation
    {
    public:
        void execute(Engine* engine) override;
    };

    //! Pushes a State onto the stack.
    class Push : public StackOperation
    {
    public:
        explicit Push(std::unique_ptr<State> state);
        void execute(Engine* engine) override;

    private:
        std::unique_ptr<State> state;
    };

    friend StackOperation;

    StateStack states;                                  //!< The stack of game States. The one at the top is updated in the run loop.
    std::queue<std::unique_ptr<StackOperation>> ops;    //!< The operations to perform on the State stack at the beginning of the next iteration.
    std::unique_ptr<sf::RenderWindow> window;           //!< The display window.
    sf::Time deltaTime;                                 //!< The amount of time per sim update.
    bool toExit;                                        //!< Whether exit has been triggered.
};


/////////////////
// Implementation

template <typename StateType>
StateType* Engine::pushState()
{
    static_assert(std::is_base_of<State, StateType>::value,
                  "StateType must be a descendant of State");

    std::unique_ptr<StateType> state = std::make_unique<StateType>(this);
    StateType* ptr = state.get();

    ops.push(std::make_unique<Push>(std::move(state)));

    return ptr;
}

}
