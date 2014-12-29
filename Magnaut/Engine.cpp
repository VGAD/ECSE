#include "Engine.h"
#include <memory>

Engine::Engine()
{
    sf::Vector2i size(800, 600);
    window = std::unique_ptr<sf::RenderWindow>(new sf::RenderWindow(sf::VideoMode(size.x, size.y), "Magnaut"));
    window->setFramerateLimit(60);
    LOG(INFO) << "Initialized window at size " << size.x << "x" << size.y;
}

Engine::~Engine()
{

}

void Engine::run()
{
    // Run the main loop
    while (window->isOpen())
    {
        updateStateStack();

        if (checkClose())
        {
            LOG(TRACE) << "Close event detected";
            window->close();
        }

        // TODO: implement proper timing code
        /*State* state = states.top().get();
        state->update(sf::Time::Zero);
        state->advance();*/

        window->clear();
        //state->render(0.0);
        window->display();
    }
}

void Engine::popState()
{
    ops.push(std::unique_ptr<StackOperation>(new Pop()));
}

void Engine::pushState(std::unique_ptr<State> state)
{
    ops.push(std::unique_ptr<StackOperation>(new Push(std::move(state))));
}

bool Engine::checkClose() const
{
    sf::Event event;

    while (window->pollEvent(event))
    {
        if (event.type == sf::Event::Closed ||
            (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
        {
            return true;
        }
    }

    return false;
}

void Engine::updateStateStack()
{
    while (!ops.empty())
    {
        StackOperation* op = ops.front().get();
        op->execute(this);
        ops.pop();
    }
}


// StateOperation implementations
void Engine::Pop::execute(Engine* engine)
{
    StateStack& states = engine->states;

    if (states.empty())
    {
        throw std::runtime_error("No States left to pop!");
    }

    LOG(INFO) << "Popping " << states.top()->getName();
    states.pop();

    if (!states.empty())
    {
        LOG(TRACE) << "Activating " << states.top()->getName();
        states.top()->activate();
    }
}


Engine::Push::Push(std::unique_ptr<State> state)
    : state(std::move(state))
{
}

void Engine::Push::execute(Engine* engine)
{
    StateStack& states = engine->states;

    if (!states.empty())
    {
        State* top = states.top().get();
        LOG(TRACE) << "Deactivating " << top->getName();
        top->deactivate();
    }

    state->engine = engine;

    LOG(INFO) << "Pushing " << state->getName();
    states.push(std::move(state));

    LOG(TRACE) << "Activating " << states.top()->getName();
    states.top()->activate();
}