#include "Engine.h"
#include "Common.h"
#include "Resources.h"
#include "WorldState.h"
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

class TestState : public WorldState
{
    virtual void activate() { LOG(TRACE) << getName() << " active"; }
    virtual void deactivate() { LOG(TRACE) << getName() << " inactive"; }

    virtual const std::string getName() { return "TestState"; }
};

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
        // Run the game
        Engine engine("ECSE Example");
        engine.pushState(std::unique_ptr<TestState>(new TestState));

        engine.run();
    }
    catch (const std::runtime_error& e)
    {
        LOG(FATAL) << e.what();
        errorMessage(e.what());
    }

    // Clean up global resources
    Resources::cleanup();

    LOG(INFO) << "Exiting";

    return 0;
}