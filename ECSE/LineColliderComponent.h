#pragma once

#include "VectorMath.h"
#include "ColliderComponent.h"

namespace ECSE {

//! A Component which stores line collision mask data.
class LineColliderComponent : public ColliderComponent
{
public:
    //! This is an extension of ColliderComponent.
    using ExtendsComponent = ColliderComponent;

    sf::Vector2f vec;       //!< The vector representing the difference between the line's start and end positions.

    //! Set the line's end point using a polar offset from the start.
    /*!
    * \param angle The angle of the line.
    * \param length The length of the line.
    */
    inline void setPolar(float angle, float length)
    {
        vec = sf::Vector2f(cos(angle) * length,
                                -sin(angle) * length);
    }

    //! Get the line's length (magnitude).
    /*!
    * \return The line's length.
    */
    inline float getLength()
    {
        return getMagnitude(vec);
    }

    //! Get the line's angle.
    /*!
    * \return The line's angle.
    */
    inline float getAngle()
    {
        return getHeading(vec);
    }
};

}
