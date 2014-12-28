#include "Resources.h"

namespace Resources
{
    ResourceManager<sf::Texture> textureManager;
    ResourceManager<sf::SoundBuffer> soundManager;
    ResourceManager<AnimationSet> animationManager;

    void cleanup()
    {
        LOG(INFO) << "Cleaning up resources";
        textureManager.clear();
        soundManager.clear();
    }
}