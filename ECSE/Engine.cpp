#include "Engine.h"
#include "Logging.h"
#include <memory>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>

namespace ECSE
{

Engine::Engine(sf::Vector2i size, std::string name, float scale, unsigned int fps, bool noRender, bool lockstep,
               unsigned int antialiasing)
{
    sf::Vector2i windowSize = {
        static_cast<int>(size.x * scale),
        static_cast<int>(size.y * scale)
    };

    if (!noRender)
    {
        sf::ContextSettings settings;
        settings.antialiasingLevel = antialiasing;

        sf::Uint32 style = sf::Style::Close;
        window = std::make_unique<sf::RenderWindow>(sf::VideoMode(windowSize.x, windowSize.y), name, style, settings);
        LOG(INFO) << "Initialized window at size " << size.x << "x" << size.y;

        if (!renderTarget.create(size.x, size.y))
        {
            throw new std::runtime_error("Failed to create render target!");
        }

        // Disabled because this causes stuttering. Without it, GPU time gets wasted a bit. Oh well.
        //window->setFramerateLimit(fps);
        rtSprite.setTexture(renderTarget.getTexture());

        inputManager.setWindow(window.get());
    }

    deltaTime = sf::seconds(1.f / float(fps));
    this->noRender = noRender;
    this->lockstep = lockstep;
    this->size = size;

    setScale(scale);

    audioManager.soundManager = &soundManager;
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

    if (lockstep)
    {
        // Perform one time step per frame
        timeStep();
    }
    else
    {
        sf::Time elapsed = clock.restart();
        if (elapsed > maxElapsed) elapsed = maxElapsed;

        accumulator += elapsed;

        // Perform steps as necessary for delta time
        while (accumulator >= deltaTime)
        {
            timeStep();
        }
    }

    if (!noRender)
    {
        float alpha = accumulator / deltaTime;

        // Draw to the render target
        renderTarget.clear();
        getActiveState().render(alpha, renderTarget);
        renderTarget.display();

        // Render scaled to screen
        window->clear();
        window->draw(rtSprite);
        window->display();
    }

    ++frames;
}

void Engine::run()
{
    running = true;

    while (running)
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
	sf::Vector2u windowSize = window->getSize();
	sf::Texture texture;
	texture.create(windowSize.x, windowSize.y);
	texture.update(*window);
	sf::Image screenshot = texture.copyToImage();
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
    if (noRender) return;

    sf::Event event;

    while (window->pollEvent(event))
    {
        handleEvent(event);
    }

    inputManager.update();

#ifdef _WINDOWS
    // SFML doesn't have the PrintScreen button. Wut.
    if (GetKeyState(VK_SNAPSHOT) & 0x8000)
    {
        saveScreenshot();
    }
#endif
}

void Engine::handleEvent(const sf::Event& e)
{
    if (e.type == sf::Event::Closed ||
        (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Escape))
    {
        running = false;
    }
}

void Engine::timeStep()
{
    State* state = &getActiveState();

    // Advance the state
    state->advance();
    ++timeSteps;

    // Remove inactive sounds
    audioManager.cullSounds();

    // We can now safely switch states because the state has advanced
    state = &updateStateStack();

    // Prepare for the next advance
    pollEvents();
    state->update(deltaTime);

    accumulator -= deltaTime;
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
