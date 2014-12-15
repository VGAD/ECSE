#include "Engine.h"
#include "Common.h"

#include <cstdio>

int main()
{
    // Redirect stdout to a file
    freopen("stdout.log", "w", stdout);

    try
    {
        Engine::run();
    }
    catch (const std::runtime_error& e)
    {
        errorMessage(e.what());
        return 1;
    }

    return 0;
}