#pragma once

#include <map>
#include <vector>
#include <SFML/System.hpp>

//! Used to store animation data
namespace ECSE
{

struct Animation
{
    //! Type for vector of animation frames indices.
    typedef std::vector<size_t> FrameVector;

    FrameVector frames; //!< Set of frame indices used for this animation.
    sf::Time frameTime; //!< The number of seconds per frame.
    bool looping;       //!< Whether this animation is looping.
};

//! Contains information about a Spritemap animation.
class AnimationSet
{
public:
    //! Load the animation data from a JSON file.
    /*!
    * \param filename The name of the file from which to load the animation.
    */
    bool loadFromFile(const std::string& filename);

    //! Get an animation from this set.
    /*!
    * \param name The name of the animation to get.
    */
    const Animation& getAnimation(const std::string& name) const;

    // Data
    sf::Vector2i frameSize;     //!< Width and height of a single frame.

private:
    std::map<std::string, Animation> anims;   //!< Map from names to animations
};


}
