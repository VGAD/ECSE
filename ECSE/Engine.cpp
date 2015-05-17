#include "Engine.h"
#include "Logging.h"
#include <memory>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>

namespace ECSE
{

Engine::Engine(sf::Vector2i size, std::string name, unsigned int fps)
{
    sf::Uint32 style = sf::Style::Close;
    window = std::make_unique<sf::RenderWindow>(sf::VideoMode(size.x, size.y), name, style);
    LOG(INFO) << "Initialized window at size " << size.x << "x" << size.y;

    if (!renderTarget.create(size.x, size.y))
    {
        throw new std::runtime_error("Failed to create render target!");
    }

    window->setFramerateLimit(fps);
    rtSprite.setTexture(renderTarget.getTexture());

    deltaTime = sf::seconds(1.f / float(fps));
}

Engine::~Engine()
{
    LOG(INFO) << "Cleaning up resources";
    textureManager.clear();
    soundManager.clear();
}

void Engine::frameStep()
{
    if (!initialized)
    {
        init();
    }

    sf::Time elapsed = clock.restart();
    if (elapsed > maxElapsed) elapsed = maxElapsed;

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

    // Draw to the render target
    renderTarget.clear();
    state->render(alpha);
    renderTarget.display();

    // Render scaled to screen
    window->clear();
    window->draw(rtSprite);
    window->display();

    ++frames;
}

void Engine::run()
{
    while (window->isOpen())
    {
        frameStep();
    }
}

void Engine::saveScreenshot()
{
    unsigned int number = 0;
    std::string formatString = "screenshot%1%.png";
    std::string filename;

    // Find an unused filename
    do
    {
        filename = str(boost::format(formatString) % number);
        number++;
    }
    while (boost::filesystem::exists(filename));

    // Save the image
    sf::Image screenshot = window->capture();
    screenshot.saveToFile(filename);

    LOG(INFO) << "Saved screenshot to \"" << filename << "\"";
}

void Engine::popState()
{
    ops.push(std::make_unique<Pop>());
}

void Engine::init()
{
    assert(!initialized);

    State& state = updateStateStack();
    state.update(deltaTime);
    initialized = true;
}

void Engine::pollEvents()
{
    sf::Event event;

    while (window->pollEvent(event))
    {
        if (event.type == sf::Event::Closed ||
            (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
        {
            toExit = true;

            break;
        }
    }

    if (toExit)
    {
        LOG(TRACE) << "Close event detected";
        window->close();
    }

#ifdef _WINDOWS
    // SFML doesn't have the PrintScreen button. Wut.
    if (GetKeyState(VK_SNAPSHOT) & 0x8000)
    {
        saveScreenshot();
    }
#endif
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

}
