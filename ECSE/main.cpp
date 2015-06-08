#include "Engine.h"
#include <cstdio>

#include "Common.h"
#include "WorldState.h"
#include "TransformSystem.h"
#include "RenderSystem.h"
#include "SpriteComponent.h"
#include "DepthComponent.h"

// Memory leak debug
#if defined(_MSC_VER) && defined(_WIN32) && defined(_DEBUG)
    #define _CRTDBG_MAP_ALLOC
    #include <stdlib.h>
    #include <crtdbg.h>

    #ifndef DBG_NEW
        #define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
        #define new DBG_NEW
    #endif
#endif

//! Contains all ECSE-related code.
namespace ECSE
{

//! Adds some text to the screen to test rendering.
class TestState : public WorldState
{
public:
    //! Construct the TestState and add the text.
    TestState(Engine* engine)
        : WorldState(engine)
    {
        world.addSystem<TransformSystem>();
        world.addSystem<RenderSystem>();

        Entity::ID test = world.createEntity();
        world.attachComponent<TransformComponent>(test)->setNextPosition(sf::Vector2f(240.f, 180.f), true);
        world.attachComponent<DepthComponent>(test);

        Spritemap* sprite = &world.attachComponent<SpriteComponent>(test)->sprite;
        sprite->setTexture(engine->textureManager.get("text.png"));

        world.registerEntity(test);
    }

    virtual void activate() override { LOG(TRACE) << getName() << " active"; }
    virtual void deactivate() override { LOG(TRACE) << getName() << " inactive"; }

    virtual std::string getName() override { return "TestState"; }
};

}

int main(int argv, char* argc[])
{
    using namespace ECSE;

#if defined(_MSC_VER) && defined(_WIN32) && defined(_DEBUG)
    // Memory leak debug
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
#endif

    init_logging(argv, argc);

    // Window size
    sf::Vector2i size(800, 600);

    try
    {
        // Run the game
        Engine engine(size, "ECSE Example");
        engine.pushState<TestState>();

        engine.run();
    }
    catch (const std::runtime_error& e)
    {
        LOG(FATAL) << e.what();
        errorMessage(e.what());
    }

    LOG(INFO) << "Exiting";

    return 0;
}