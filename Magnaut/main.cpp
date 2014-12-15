#include "Engine.h"

int main()
{
	Engine::init();
	Engine::loop();
	Engine::cleanup();

	return 0;
}