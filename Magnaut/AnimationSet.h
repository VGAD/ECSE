#pragma once

#include <map>
#include <vector>
#include <SFML/System.hpp>

//! Used to store animation data
struct Animation
{
    //! Type for vector of animation frames indices.
    typedef std::vector<size_t> FrameVector;

    FrameVector frames;
    sf::Time frameTime;
    bool looping;
};

//! Contains information about a Spritemap animation.
class AnimationSet
{
public:
    //! Load the animation data from a JSON file.
    /*!
      \param fileName The name of the file from which to load the animation.
    */
    bool loadFromFile(const std::string& filename);

    //! Get an animation from this set.
    const Animation& getAnimation(const std::string& name) const;

    // Data
    sf::Vector2i frameSize;     //!< Width and height of a single frame.

private:
    std::map<std::string, Animation> anims;   //!< Map from names to animations
};
