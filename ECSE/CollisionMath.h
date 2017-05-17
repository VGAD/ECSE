//! \file CollisionMath.h Contains functions for solving collision times.

#include "SFML/System.hpp"

#pragma once

namespace ECSE
{

//! Represents an intersection between two lines.
struct LineIntersection
{
    bool intersection = false;          //!< Whether there was an intersection. If this is false, then the lines are parallel and t, u and point are invalid.
    bool strictIntersection = false;    //!< False if the intersection occurred between the lines, but not the segments.
    float t;                            //!< Distance of point along first line in range [0, 1].
    float u;                            //!< Distance of point along second line in range [0, 1].
};

//! Find the point of intersection between two line segments.
/*!
* \param startA The start of the first line.
* \param endA The end of the first line.
* \param startB The start of the second line.
* \param endB The end of the first line.
* \return A LineIntersection struct containing intersection data.
*/
LineIntersection findLineIntersection(sf::Vector2f startA, sf::Vector2f endA,
                                      sf::Vector2f startB, sf::Vector2f endB);

//! Project a point onto a line segment.
/*!
* \param point The point to project onto the line.
* \param start The start of the line segment.
* \param end The end of the line segment.
* \return The distance of the point along the line.
*/
float projectPointOntoLine(sf::Vector2f& point, sf::Vector2f start, sf::Vector2f end);

//! Find the time of collision between a moving circle and a stationary circle.
/*!
* \param centerA The center of the first circle.
* \param radiusA The radius of the first circle.
* \param centerB The center of the second circle.
* \param radiusB The radius of the second circle.
* \param velocity The velocity of the first circle.
* \param time The time at which the collision occurred, or <0 if no collision.
* \param normal The normal of the collision direction (direction is from circle A to circle B). Value is meaningless if no collision.
*/
void circleCircle(sf::Vector2f centerA, float radiusA, sf::Vector2f centerB, float radiusB,
                  sf::Vector2f velocity, float &time, sf::Vector2f &normal);

//! Find the time of collision between a moving circle and a stationary line.
/*!
* \param centerA The center of the circle.
* \param radiusA The radius of the circle.
* \param startB The start point of the line.
* \param endB The end point of the line.
* \param time The time at which the collision occurred, or <0 if no collision.
* \param normal The normal of the collision direction (direction is from circle to line). Value is meaningless if no collision.
*/
void circleLine(sf::Vector2f centerA, float radiusA, sf::Vector2f startB, sf::Vector2f endB,
                sf::Vector2f velocity, float &time, sf::Vector2f &normal);

}
