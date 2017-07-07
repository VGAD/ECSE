#include "gtest/gtest.h"
#include "ECSE/AudioManager.h"
#include "TestUtils.h"
#include <chrono>
#include <thread>

class AudioManagerTest : public ::testing::Test
{
public:
    ECSE::AudioManager manager;
};

TEST_F(AudioManagerTest, AddSoundTest)
{
    sf::SoundBuffer buffer;
    auto sound = manager.createSound(buffer);

    ASSERT_NE(nullptr, sound.get());
    ASSERT_EQ(&buffer, sound->getBuffer());
}

TEST_F(AudioManagerTest, AddTooManySoundsTest)
{
    sf::SoundBuffer buffer;

    for (int i = 0; i < ECSE_MAX_SOUND_COUNT; ++i)
    {
        auto sound = manager.createSound(buffer);
    }

    ASSERT_THROW(manager.createSound(buffer), std::runtime_error);
}

TEST_F(AudioManagerTest, ActiveCountTest)
{
    sf::SoundBuffer buffer;

    for (int i = 0; i < 3; ++i)
        auto sound = manager.createSound(buffer);

    auto music = manager.createMusic();

    ASSERT_EQ(4, manager.getActiveCount());
}

TEST_F(AudioManagerTest, RetainSharedTest)
{
    sf::SoundBuffer buffer;
    auto sound = manager.createSound(buffer);
    auto music = manager.createMusic();

    manager.cullSounds();

    ASSERT_EQ(2, manager.getActiveCount());
}

TEST_F(AudioManagerTest, RetainNotStoppedTest)
{
    sf::SoundBuffer buffer;
    sf::Int16 data[10];

    // Zero out sound data so it doesn't make an annoying pop when tests are run
    for (int i = 0; i < 10; ++i)
    {
        data[i] = 0;
    }

    // This test will only fail if we take more than 10 seconds to run it, because the sound will run out
    buffer.loadFromSamples(data, 10, 1, 1);

    // sound/music will only be held by the audio manager outside of this scope
    {
        auto sound = manager.createSound(buffer);
        sound->play();
        sound->pause();
    }

    manager.cullSounds();

    ASSERT_EQ(1, manager.getActiveCount()) << "This test may have failed because things are running REALLY slowly";
}

TEST_F(AudioManagerTest, RemoveNotPlayingTest)
{
    sf::SoundBuffer buffer;
    manager.createSound(buffer);
    manager.createMusic();

    manager.cullSounds();

    ASSERT_EQ(0, manager.getActiveCount());
}

TEST_F(AudioManagerTest, RemoveFinishedPlayingTest)
{
    sf::SoundBuffer buffer;
    sf::Int16 data[10];

    // Zero out sound data so it doesn't make an annoying pop when tests are run
    for (int i = 0; i < 10; ++i)
    {
        data[i] = 0;
    }

    // 0.1 seconds to finish playing the clip
    buffer.loadFromSamples(data, 10, 1, 100);

    // Sound will only be held by the audio manager outside of this scope
    {
        auto sound = manager.createSound(buffer);
        sound->play();

        // Spin until sound is done playing
        while (sound->getStatus() == sf::Sound::Playing) {}
    }

    manager.cullSounds();

    ASSERT_EQ(0, manager.getActiveCount());
}

TEST_F(AudioManagerTest, TestVolumeOnAdd)
{
    manager.setMasterVolume(0.5f);
    manager.setCategoryVolume(1, 0.2f);
    manager.setCategoryVolume(2, 0.1f);

    sf::SoundBuffer buffer;
    auto sound = manager.createSound(buffer, 1);
    auto music = manager.createMusic(2);

    manager.cullSounds();

    ASSERT_FLOAT_EQ(0.1f, sound->getVolume());
    ASSERT_FLOAT_EQ(0.05f, music->getVolume());
}

TEST_F(AudioManagerTest, TestVolumeAfterAdd)
{
    manager.setMasterVolume(0.5f);
    manager.setCategoryVolume(1, 0.2f);
    manager.setCategoryVolume(2, 0.1f);

    sf::SoundBuffer buffer;
    auto sound = manager.createSound(buffer, 1);
    auto music = manager.createMusic(2);

    manager.setCategoryVolume(1, 0.4f);
    manager.setCategoryVolume(2, 0.6f);

    manager.cullSounds();

    ASSERT_FLOAT_EQ(0.2f, sound->getVolume());
    ASSERT_FLOAT_EQ(0.3f, music->getVolume());
}