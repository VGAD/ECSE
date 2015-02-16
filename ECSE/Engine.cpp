#include "Engine.h"
#include <memory>

Engine::Engine(std::string name, unsigned int fps)
{
    sf::Vector2i size(800, 600);
    window = std::unique_ptr<sf::RenderWindow>(new sf::RenderWindow(sf::VideoMode(size.x, size.y), name));
    LOG(INFO) << "Initialized window at size " << size.x << "x" << size.y;

    deltaTime = sf::seconds(1.f / float(fps));
}

Engine::~Engine()
{
    LOG(INFO) << "Cleaning up resources";
    textureManager.clear();
    soundManager.clear();
}

void Engine::run()
{
    sf::Time accumulator = sf::Time::Zero;
    sf::Clock clock;

    // Do this to make sure we have an initial State which is ready to advance
    updateStateStack();
    states.top()->update(deltaTime);

    // Run the main loop
    while (window->isOpen())
    {
        sf::Time elapsed = clock.restart();
        accumulator += elapsed;

        State* state = &getActiveState();

        while (accumulator >= deltaTime)
        {
            // Advance the state
            state->advance();
            ++ticks;

            // We can now safely switch states because the state has advanced
            state = &updateStateStack();

            // Prepare for the next advance
            pollEvents();
            state->update(deltaTime);

            accumulator -= deltaTime;
        }

        float alpha = accumulator / deltaTime;

        // Draw to screen
        window->clear();
        state->render(alpha);
        window->display();
    }
}

void Engine::popState()
{
    ops.push(std::unique_ptr<StackOperation>(new Pop()));
}

void Engine::pollEvents()
{
    sf::Event event;

    while (window->pollEvent(event))
    {
        if (event.type == sf::Event::Closed ||
            (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
        {
            LOG(TRACE) << "Close event detected";
            window->close();

            break;
        }
    }
}

State& Engine::updateStateStack()
{
    while (!ops.empty())
    {
        StackOperation* op = ops.front().get();
        op->execute(this);
        ops.pop();
    }

    return getActiveState();
}

State& Engine::getActiveState() const
{
    if (states.empty())
    {
        throw std::runtime_error("No State left in the stack!");
    }

    return *states.top();
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