#pragma once

#include "ECSE/WorldState.h"

namespace ReplayDemo
{

//! Demonstrates replay functionality.
class ReplayDemoState : public ECSE::WorldState
{
public:
    //! Construct the state.
    explicit ReplayDemoState(ECSE::Engine* engine);

    //! Update state.
    void update(sf::Time deltaTime) override;

    //! Render help text.
    void render(float alpha, sf::RenderTarget &renderTarget) override;

    //! Get the state's name.
    std::string getName() override { return "ReplayDemoState"; }

private:
    //! The demo recording stream.
    std::stringstream demoStream;

    //! Font used for text.
    sf::Font font;

    //! Text shape used for rendering text.
    sf::Text text;

    //! The demo state.
    enum DemoState {
        Start,
        Record,
        Replay,
        Done,
        StateCount
    } currentState = Start;

    //! The ball's ID.
    ECSE::Entity::ID ballId;

    //! Reset the ball's position.
    void resetBall();
};

}