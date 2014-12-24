#include "Global.h"
#include "Engine.h"
#include "Common.h"
#include "AnimationSet.h"

#include <cstdio>

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

int main(int argv, char* argc[])
{
#if defined(_MSC_VER) && defined(_WIN32) && defined(_DEBUG)
    // Memory leak debug
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
#endif

    init_logging(argv, argc);

    try
    {
        // Run the game, then clean up
        Engine engine;
        engine.run();
    }
    catch (const std::runtime_error& e)
    {
        LOG(FATAL) << e.what();
        errorMessage(e.what());
    }

    Magnaut::cleanup();

    LOG(INFO) << "Exiting Magnaut";

    return 0;
}