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
#include "InputManager.h"

namespace ECSE
{

//! Contains the base game information and procedures, such as the game loop and resource management.
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
    * \param noRender If true, no window is created and rendering is not performed.
    * \param lockstep If true, one timestep is simulated per frame no matter what.
    */
    explicit Engine(sf::Vector2i size, std::string name = "", unsigned int fps = 60,
                    bool noRender = false, bool lockstep = false);

    //! Create the game window and initialize the Engine with a scaled resolution.
    /*!
    * \param size The unscaled window dimensions.
    * \param scale The scale of the window.
    * \param name The window name.
    * \param fps The target FPS.
    * \param noRender If true, no window is created and rendering is not performed.
    * \param lockstep If true, one timestep is simulated per frame no matter what.
    */
    explicit Engine(sf::Vector2i size, float scale, std::string name = "", unsigned int fps = 60,
                    bool noRender = false, bool lockstep = false);

    //! Destroy all game objects, clean up resources and stop the game.
    virtual ~Engine();

    //! Perform a full frame step, including running update/advance pairs and one render step.
    virtual void frameStep();

    //! Run the game loop, calling frameStep() repeatedly until stopped.
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

    //! Get the size of the displayed area.
    /*!
    * \return The size.
    */
    inline sf::Vector2i getSize() const
    {
        return size;
    }

    //! Get the size of the displayed area, taking scaling into account.
    /*!
    * E.g. if the size is 640x480 and the scale is 2, then this will return 320x240.
    * The result is rounded up to the nearest pixel.
    *
    * \return The unscaled size.
    */
    inline sf::Vector2i getUnscaledSize() const
    {
        auto scale = getScale();
        return sf::Vector2i(static_cast<int>(ceil(size.x / scale)),
                            static_cast<int>(ceil(size.y / scale)));
    }

    //! Get the number of update/advance time step pairs since this started.
    inline size_t getTimeSteps() const
    {
        return timeSteps;
    }

    //! Get the number of frames drawn since this started.
    inline size_t getFrames() const
    {
        return frames;
    }

    //! Stop the game loop at the end of this iteration if it is running.
    inline void stop()
    {
        running = false;
    }

    //! Check if the game loop is running.
    /*!
    * \return Whether the loop is running.
    */
    inline bool isRunning()
    {
        return running;
    }

    //! Check if the game window has focus.
    /*!
    * \return True if the game window has focus.
    */
    inline bool hasFocus()
    {
        return window->hasFocus();
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
    InputManager inputManager;                      //!< Keeps track of user input.
    ResourceManager<sf::Texture> textureManager;    //!< Keeps track of loaded Textures.
    ResourceManager<sf::SoundBuffer> soundManager;  //!< Keeps track of loaded SoundBuffers.
    ResourceManager<AnimationSet> animationManager; //!< Keeps track of loaded AnimationSets.


protected:
    //! Perform a single timestep (advance and then update).
    virtual void timeStep();

    //! Handle an event.
    virtual void handleEvent(const sf::Event& e);

private:
    ////////////
    // Functions

    //! Call this once a state has been pushed to the stack with pushState().
    /*!
    * This activates the state and updates it so it is ready to be advanced in frameStep().
    * This should only be called once!
    */
    void init();

    //! Poll events and handle them.
    void pollEvents();

    //! Update the State stack.
    /*!
    * \return A reference to the State on top of the stack.
    */
    State& updateStateStack();


    ///////
    // Data

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

    sf::Clock clock;                                    //!< Clock used to measure frame time.
    sf::Time accumulator = sf::Time::Zero;              //!< Accumulator used to measure time elapsed in rendering.
    sf::Time maxElapsed = sf::seconds(0.25f);           //!< Maximum amount of time to be simulated before rendering (to avoid spiral of doom for expensive simulation).
    sf::Time deltaTime;                                 //!< The amount of time per sim update.

    size_t frames = 0;                                  //!< Number of frames since the engine was started.
    size_t timeSteps = 0;                               //!< Number of update/advance time step pairs since the engine was started.

    StateStack states;                                  //!< The stack of game States. The one at the top is updated in the run loop.
    std::queue<std::unique_ptr<StackOperation>> ops;    //!< The operations to perform on the State stack at the beginning of the next iteration.

    bool initialized = false;                           //!< Whether the first state has been activated yet.
    bool running = false;                               //!< Whether the game loop is running.
    bool noRender = false;                              //!< If true, no window is created and rendering is not performed.
    bool lockstep = false;                              //!< If true, one timestep is simulated per frame no matter what.

    sf::Vector2i size;                                  //!< The size of the display area.
    sf::Sprite rtSprite;                                //!< Sprite which is used to update the window display.
    sf::RenderTexture renderTarget;                     //!< Offscreen buffer used to update the window display.
    std::unique_ptr<sf::RenderWindow> window;           //!< The display window.
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
