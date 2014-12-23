#include "Global.h"

namespace Magnaut
{
    ResourceManager<sf::Texture> textureManager;
    ResourceManager<sf::SoundBuffer> soundManager;

    void cleanup()
    {
        LOG(TRACE) << "Cleaning up";
        textureManager.clear();
        soundManager.clear();
    }
}