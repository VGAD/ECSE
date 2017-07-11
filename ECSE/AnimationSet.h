#pragma once

#include <map>
#include <vector>
#include <SFML/System.hpp>

namespace ECSE
{

//! Used to store animation data.
struct Animation
{
    //! Type for vector of animation frames indices.
    typedef std::vector<size_t> FrameVector;

    FrameVector frames; //!< Set of frame indices used for this animation.
    sf::Time frameTime; //!< The number of seconds per frame.
    bool looping;       //!< Whether this animation is looping.

    //! Get the total length of the animation.
    inline sf::Time getLength() const
    {
        return static_cast<float>(frames.size()) * frameTime;
    }
};

//! Contains information about a Spritemap animation.
class AnimationSet
{
public:
    //! Load the animation data from a JSON file.
    /*!
    * \param filename The name of the file from which to load the animation.
    * \return True if the animation was loaded successfully.
    */
    bool loadFromFile(const std::string& filename);

    //! Get an animation from this set.
    /*!
    * \param name The name of the animation to get.
    * \return A pointer to the animation.
    */
    const Animation* getAnimation(const std::string& name) const;

    //! Get the list of animation names.
    /*!
    * \return A vector containing the names of every animation in the set.
    */
    std::vector<std::string> getAnimationNames() const;

    // Data
    sf::Vector2i frameSize;     //!< Width and height of a single frame.

private:
    std::map<std::string, Animation> anims;   //!< Map from names to animations
};

}
