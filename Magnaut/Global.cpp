#include "Global.h"

namespace Magnaut
{
    ResourceManager<sf::Texture> textureManager;
    ResourceManager<sf::SoundBuffer> soundManager;

    void cleanup()
    {
        LOG(INFO) << "Cleaning up resources";
        textureManager.clear();
        soundManager.clear();
    }
}