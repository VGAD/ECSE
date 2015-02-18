#include "AnimationSet.h"
#include "Common.h"

#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

bool AnimationSet::loadFromFile(const std::string &filename)
{
    using boost::property_tree::ptree;
    using boost::property_tree::read_json;

    ptree pt;
    try
    {
        read_json(filename, pt);

        frameSize.x = pt.get<int>("frameWidth");
        frameSize.y = pt.get<int>("frameHeight");

        // Loop through listed animations
        for (const ptree::value_type& animVal : pt.get_child("animations"))
        {
            const std::string& name = animVal.first;
            const ptree& animTree = animVal.second;

            if (anims.find(name) != anims.end())
            {
                LOG(WARNING) << "Tried to add animation \"" + name + "\" more than once in " + filename + "! Ignoring...";
                continue;
            }

            Animation& anim = anims[name];

            // Basic animation properties
            float frameTime = animTree.get<float>("frameTime", 0.0);
            anim.frameTime = sf::seconds(frameTime);
            anim.looping = animTree.get<bool>("looping", false);

            // Frames of animation
            for (const ptree::value_type& framePair : animTree.get_child("frames"))
            {
                anim.frames.push_back(framePair.second.get_value<int>());
            }
        }
    }
    catch (boost::property_tree::ptree_error &e)
    {
        std::string msg = "";
        msg += "Failed to load animation from " + filename + ": " + e.what();

        throw std::runtime_error(msg);
    }

    return true;
}

const Animation& AnimationSet::getAnimation(const std::string& name) const
{
    auto it = anims.find(name);
    if (it == anims.end())
    {
        throw std::runtime_error("Couldn't find animation \"" + name + "\"");
    }

    return it->second;
}