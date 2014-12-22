#include "Global.h"
#include "Engine.h"
#include "Common.h"

#include <cstdio>

int main()
{
    // Redirect stdout to a file
    freopen("stdout.log", "w", stdout);

    try
    {
        // Run the game, then clean up
        Engine engine;
        engine.run();
        Magnaut::cleanup();
    }
    catch (const std::runtime_error& e)
    {
        LOG(fatal) << e.what();
        errorMessage(e.what());
        return 1;
    }

    return 0;
}