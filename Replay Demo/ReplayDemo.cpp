#include "ECSE/Engine.h"
#include "ECSE/TransformSystem.h"
#include "ECSE/SpecializationSystem.h"
#include "ECSE/SpecializationComponent.h"
#include "ReplayDemo.h"
#include "Bindings.h"
#include "ControllableBall.h"

namespace ReplayDemo
{

ReplayDemoState::ReplayDemoState(ECSE::Engine* engine)
    : WorldState(engine)
{
    world.addSystem<ECSE::SpecializationSystem>();
    world.addSystem<ECSE::TransformSystem>();

    // Set up the ball
    ballId = world.createEntity();
    world.attachComponent<ECSE::TransformComponent>(ballId);
    auto specComponent = world.attachComponent<ECSE::SpecializationComponent>(ballId);
    specComponent->setSpecialization(std::make_unique<ControllableBall>());
    world.registerEntity(ballId);

    resetBall();

    // Set up text
    font.loadFromFile("C:/Windows/Fonts/arial.ttf");
    text.setFont(font);
    text.setFillColor(sf::Color(255, 255, 255, 255));
    text.setOutlineColor(sf::Color(0, 0, 0, 255));
    text.setOutlineThickness(1.f);
    text.setCharacterSize(16);
}

void ReplayDemoState::update(sf::Time deltaTime)
{
    auto &inputMan = engine->inputManager;

    // Adance button was pressed this frame
    bool advanced = inputMan.getIntDelta(Bindings::Advance) == 1;

    // Waiting to start and ball was moved
    bool startedMoving = currentState == DemoState::Start
        && (inputMan.getIntValue(Bindings::MoveX) || inputMan.getIntValue(Bindings::MoveY));

    // Replay ended
    bool replayDone = currentState == DemoState::Replay && !inputMan.isPlayingDemo();

    if (advanced || startedMoving || replayDone)
    {
        switch (currentState)
        {
        case DemoState::Start:
            inputMan.startRecording(demoStream);
            break;

        case DemoState::Record:
            inputMan.stopRecording();

            // Rewind demo stream and start playing it
            resetBall();
            demoStream.seekg(0);
            inputMan.playDemo(demoStream);

            break;

        case DemoState::Replay:
            inputMan.stopDemo();
            break;

        case DemoState::Done:
            demoStream = std::stringstream();
            resetBall();
            break;
        }

        // Advance the state, cycling back to the first one
        currentState = static_cast<DemoState>((currentState + 1) % DemoState::StateCount);
    }

    ECSE::WorldState::update(deltaTime);
}

void ReplayDemoState::render(float alpha, sf::RenderTarget &renderTarget)
{
    ECSE::WorldState::render(alpha, renderTarget);

    text.setPosition(sf::Vector2f(10.f, 10.f));
    std::string str;
    switch (currentState)
    {
    case Start:
        str = "Use arrow keys to move the ball.\nRecording will start as soon as you move the ball or press SPACE.";
        break;

    case Record:
        str = "Recording in progress.\nUse arrow keys to move the ball.\nPress SPACE to stop recording and play it back.";
        break;

    case Replay:
        str = "Playing back recording...";
        break;

    case Done:
        str = "Recording finished.\nPress SPACE to restart.";
        break;
    }
    text.setString(str);
    renderTarget.draw(text);
}

void ReplayDemoState::resetBall()
{
    auto entity = world.getEntity(ballId);
    auto transform = entity->getComponent<ECSE::TransformComponent>();
    transform->setLocalPosition(sf::Vector2f(400.f, 300.f));
}

}