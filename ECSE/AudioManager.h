#pragma once

#include <SFML/Audio.hpp>
#include <memory>
#include <unordered_set>
#include <map>
#include <array>
#include "ResourceManager.h"

// SFML recommends keeping this to 256 or lower, so raise at your own risk.
#ifndef ECSE_MAX_SOUND_COUNT
#define ECSE_MAX_SOUND_COUNT 256
#endif

namespace ECSE
{

//! Handles allocation and volume levels of sound and music.
class AudioManager
{
    friend class Engine;

public:
    //! Construct the audio manager.
    explicit AudioManager();

    //! Create a sound.
    /*!
    * This function returns a shared pointer. When only the sound manager holds the pointer and
    * the sound has stopped playing, it will be destroyed. Note that the underlying buffer will
    * not be deallocated; just the sf::Sound itself.
    *
    * \param buffer The sound buffer.
    * \param category An optional user-defined sound category.
    * \return A shared pointer to the sound.
    */
    std::shared_ptr<sf::Sound> createSound(const sf::SoundBuffer &buffer, uint8_t category = 0);

    //! Convenience function create a sound from a filename.
    /*!
    * This function loads the data into a buffer in the sound resource manager if it's been set
    * for this audio manager (if you're using the one built into an ECSE engine, it will be set).
    * Otherwise, this function will throw an exception.
    *
    * This function returns a shared pointer. When only the sound manager holds the pointer and
    * the sound has stopped playing, it will be destroyed. Note that the underlying buffer will
    * not be deallocated; just the sf::Sound itself.
    *
    * \param filename The filename of the sound file.
    * \param category An optional user-defined sound category.
    * \return A shared pointer to the sound.
    */
    std::shared_ptr<sf::Sound> createSound(std::string filename, uint8_t category = 0);

    //! Create a streaming music source.
    /*!
    * This function returns a shared pointer. When only the sound manager holds the pointer and
    * the sound has stopped playing, it will be destroyed. Note that the underlying buffer will
    * not be deallocated; just the sf::Sound itself.
    *
    * \param category An optional user-defined sound category.
    * \return A shared pointer to the music.
    */
    std::shared_ptr<sf::Music> createMusic(uint8_t category = 0);

    //! Convenience function to create a streaming music source from a file.
    /*!
    * This function returns a shared pointer. When only the sound manager holds the pointer and
    * the sound has stopped playing, it will be destroyed. Note that the underlying buffer will
    * not be deallocated; just the sf::Sound itself.
    *
    * \param filename The filename of the music file.
    * \param category An optional user-defined sound category.
    * \return A shared pointer to the music.
    */
    std::shared_ptr<sf::Music> createMusic(std::string filename, uint8_t category = 0);

    //! Set the master volume.
    /*!
    * \param volume The new master volume.
    */
    inline void setMasterVolume(float volume)
    {
        masterVolume = volume;
        updateAllVolumes();
    }

    //! Set the volume of all sounds in a category.
    /*!
    * \param volume The new category volume.
    */
    inline void setCategoryVolume(uint8_t category, float volume)
    {
        volumes[category] = volume;
        updateAllVolumes();
    }

    //! Remove sounds that are no longer active.
    /*!
    * Clears out sounds that are no longer in use.
    */
    void cullSounds();

    //! Get the number of active sounds.
    /*!
    * \return The number of active sounds (either playing or being held in memory by another class).
    */
    inline size_t getActiveCount() const
    {
        return sounds.size() + musicSources.size();
    }

    //! Get the master volume.
    /*!
    * \return The master volume.
    */
    inline float getMasterVolume() const
    {
        return masterVolume;
    }

    //! Get a category's volume.
    /*!
    * \return The category's volume.
    */
    inline float getCategoryVolume(uint8_t category) const
    {
        return volumes[category];
    }

protected:
    //! The optional sound resource manager.
    ResourceManager<sf::SoundBuffer> *soundManager = nullptr;

private:
    //! Holds info for an active sound.
    struct SoundData
    {
        SoundData(std::shared_ptr<sf::Sound> sound, uint8_t category)
        {
            this->sound = sound;
            this->category = category;
        }

        std::shared_ptr<sf::Sound> sound;   //!< The sound.
        uint8_t category;                   //!< Its category.
    };

    //! Holds info for an active music source.
    struct MusicData
    {
        MusicData(std::shared_ptr<sf::Music> music, uint8_t category)
        {
            this->music = music;
            this->category = category;
        }

        std::shared_ptr<sf::Music> music;   //!< The music.
        uint8_t category;                   //!< Its category.
    };

    //! Volume levels for each category.
    std::array<float, std::numeric_limits<uint8_t>::max()> volumes;

    //! The master volume.
    float masterVolume = 1.f;

    //! All active sounds.
    // TODO: this could be an unordered_set if we provide a hashing function for SoundData
    std::vector<SoundData> sounds;

    //! All active music.
    // TODO: this could be an unordered_set if we provide a hashing function for SoundData
    std::vector<MusicData> musicSources;

    //! Update volumes of every sound and music source.
    void updateAllVolumes();

    //! Get the volume of a sound in a given category
    /*!
    * \param category The sound category.
    * \return The volume, including master and category adjustments.
    */
    inline float getFinalVolume(uint8_t category)
    {
        return volumes[category] * masterVolume;
    }
};

}