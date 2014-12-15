#include "Engine.h"
#include "Common.h"

int main()
{
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