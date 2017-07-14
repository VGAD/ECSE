#include "AudioManager.h"

namespace ECSE
{

AudioManager::AudioManager()
{
    for (size_t i = 0; i < volumes.size(); ++i)
    {
        volumes[i] = 1.f;
    }
}

std::shared_ptr<sf::Sound> AudioManager::createSound(const sf::SoundBuffer &buffer, uint8_t category)
{
    if (getActiveCount() >= ECSE_MAX_SOUND_COUNT)
    {
        throw std::runtime_error("Maximum sound count exceeded!");
    }

    auto sound = std::make_shared<sf::Sound>(buffer);
    sound->setVolume(getFinalVolume(category));
    sounds.push_back(SoundData(sound, category));

    return sound;
}

std::shared_ptr<sf::Sound> AudioManager::createSound(std::string filename, uint8_t category)
{
    if (soundManager == nullptr)
    {
        throw std::runtime_error("No sound resource manager set in AudioManager!");
    }

    auto &buffer = soundManager->get(filename);
    return createSound(buffer, category);
}

std::shared_ptr<sf::Music> AudioManager::createMusic(uint8_t category)
{
    if (getActiveCount() >= ECSE_MAX_SOUND_COUNT)
    {
        throw std::runtime_error("Maximum sound count exceeded!");
    }

    auto music = std::make_shared<sf::Music>();
    music->setVolume(getFinalVolume(category));
    musicSources.push_back(MusicData(music, category));

    return music;
}

std::shared_ptr<sf::Music> AudioManager::createMusic(std::string filename, uint8_t category)
{
    auto music = createMusic(category);
    music->openFromFile(filename);

    return music;
}

void AudioManager::cullSounds()
{
    for (auto it = sounds.begin(); it != sounds.end();)
    {
        auto &data = *it;
        auto &sound = data.sound;

        // Remove any sounds that are not playing and only owned by the audio manager
        if (sound->getStatus() == sf::Sound::Status::Stopped && sound.use_count() == 1)
        {
            sounds.erase(it);
        }
        else
        {
            ++it;
        }
    }

    for (auto it = musicSources.begin(); it != musicSources.end();)
    {
        auto &data = *it;
        auto &music = data.music;

        // Remove any music sources that are not playing and only owned by the audio manager
        if (music->getStatus() == sf::Sound::Status::Stopped && music.use_count() == 1)
        {
            musicSources.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void AudioManager::updateAllVolumes()
{
    for (auto soundData : sounds)
    {
        soundData.sound->setVolume(getFinalVolume(soundData.category));
    }

    for (auto musicData : musicSources)
    {
        musicData.music->setVolume(getFinalVolume(musicData.category));
    }
}

}