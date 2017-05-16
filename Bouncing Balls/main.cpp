#include "ECSE/Engine.h"
#include "ECSE/Common.h"
#include "ECSE/WorldState.h"
#include "ECSE/TransformSystem.h"
#include "ECSE/RenderSystem.h"
#include "ECSE/SpriteComponent.h"
#include "ECSE/DepthComponent.h"
#include "BouncingBalls.h"

/*

This example demonstrates continuous collision detection in ECSE.

*/


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

using namespace ECSE;

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
        Engine engine(size, "Circle Collision Example");
        engine.pushState<BouncingBalls::BouncingBallsState>();

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