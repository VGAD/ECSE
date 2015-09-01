#pragma once

#include "Component.h"
#include <bitset>

#ifndef ECSE_TAG_COUNT
#define ECSE_TAG_COUNT 32
#endif

namespace ECSE {

//! A Component which stores tags associated with the entity.
/*!
* This is useful to differentiate entities of different types, such as enemies, players,
* bullets, etc. Tags are numerical, so you should create an enum to differentiate your
* tags. If you need more than 32 tags, redefine ECSE_TAG_COUNT to your required number.
*/
class TagComponent : public Component
{
public:
    //! Add a tag to the component.
    /*!
    * \param tag The tag to add.
    */
    void addTag(size_t tag) { tags.set(tag, true); }

    //! Remove a tag from the component.
    /*!
    * \param tag The tag to remove.
    */
    void removeTag(size_t tag) { tags.set(tag, false); }

    //! Check if the component has a tag.
    /*!
    * \param tag The tag to check.
    * \return True if the component has the tag.
    */
    bool hasTag(size_t tag) { return tags.test(tag); }

private:
    std::bitset<ECSE_TAG_COUNT> tags;   //!< The set of tags on this component.
};

}
