#include "VectorMath.h"
#include "CollisionMath.h"

namespace ECSE
{

LineIntersection findLineIntersection(sf::Vector2f startA, sf::Vector2f endA,
                                      sf::Vector2f startB, sf::Vector2f endB)
{
    // See http://stackoverflow.com/a/565282/858878
    sf::Vector2f lineA = endA - startA;
    sf::Vector2f lineB = endB - startB;
    
    float cross1 = get2DCrossProduct(startB - startA, lineB);
    float cross2 = get2DCrossProduct(startB - startA, lineA);
    float cross3 = get2DCrossProduct(lineA, lineB);

    LineIntersection result;

    // Techically there could be infinitely many points, but for our purposes we don't care
    if (cross3 == 0)
    {
        return result;
    }

    result.intersection = true;
    result.t = cross1 / cross3;
    result.u = cross2 / cross3;

    result.strictIntersection = !(result.t < 0 || result.t > 1 || result.u < 0 || result.u > 1);

    return result;
}

float projectPointOntoLine(sf::Vector2f& point, sf::Vector2f start, sf::Vector2f end)
{
    sf::Vector2f startToPoint = point - start;
    sf::Vector2f startToEnd = end - start;

    float dot = getDotProduct(startToPoint, startToEnd);
    float t = dot / getSqrMagnitude(startToEnd);

    point = start + startToEnd * t;

    return t;
}

float circleCircle(sf::Vector2f centerA, float radiusA, sf::Vector2f centerB,
                    float radiusB, sf::Vector2f velocity)
{
    sf::Vector2f distVec = centerB - centerA;
    float sumRadii = radiusA + radiusB;

    // Already colliding
    if (getSqrMagnitude(distVec) < sumRadii * sumRadii) return 0.f;

    float centerDist = getMagnitude(distVec);
    float moveDist = getMagnitude(velocity);
    float betweenDist = centerDist - sumRadii;

    // Not moving far enough to close the distance
    if (moveDist < betweenDist) return -1.f;

    sf::Vector2f moveNormal(velocity);
    normalize(moveNormal);

    // Distance moved toward collider B
    float towardDist = getDotProduct(moveNormal, distVec);

    // Not moving toward each other
    if (towardDist <= 0) return -1.f;

    float shortestDistSqr = (centerDist * centerDist) - (towardDist * towardDist);
    float sumRadiiSqr = sumRadii * sumRadii;

    // colliderA will never get close enough to colliderB
    if (shortestDistSqr >= sumRadiiSqr) return -1.f;

    // sumRadiiSqr is the hypotenuse squared, shortestDistSqr one of the other sides squared
    // and this is the third (a^2 = c^2 - b^2)
    float thirdSideSqr = sumRadiiSqr - shortestDistSqr;

    // Can't take a negative square root
    if (thirdSideSqr < 0) return -1.f;

    // The actual distance travelled forward to collide
    float collideDist = towardDist - sqrt(thirdSideSqr);

    // Not going to move that far
    if (moveDist < collideDist) return -1.f;

    return collideDist / moveDist;
}

float circleLine(sf::Vector2f centerA, float radiusA, sf::Vector2f startB,
                 sf::Vector2f endB, sf::Vector2f velocity)
{
    // Based loosely on http://ericleong.me/research/circle-line/

    // Find closest point on line to circle
    sf::Vector2f closeToCircle = centerA;
    float t = projectPointOntoLine(closeToCircle, startB, endB);

    float radiusASqr = radiusA * radiusA;
    float distSqr = getSqrMagnitude(centerA - closeToCircle);

    // Circle is already touching the line
    if (distSqr < radiusASqr && t >= 0 && t <= 1) return 0.f;

    auto intersectResult = findLineIntersection(startB, endB,
                                                centerA, centerA + velocity);

    // Circle is moving parallel to line, so check the first endpoint to be passed
    if (!intersectResult.intersection)
    {
        float dotStart = getDotProduct(startB - centerA, velocity);
        float dotEnd = getDotProduct(endB - centerA, velocity);

        sf::Vector2f* endpoint;

        // Already passed start point
        if (dotStart < 0)
        {
            // Aleady passed end point as well
            if (dotEnd < 0)
            {
                return -1.f;
            }

            // Already passed start, but not end
            endpoint = &endB;
        }
        // Already passed end, but not start
        else if (dotEnd < 0)
        {
            endpoint = &startB;
        }
        // Haven't passed either, so pick the nearest
        else
        {
            endpoint = (dotStart < dotEnd) ? &startB : &endB;
        }

        return circleCircle(centerA, radiusA, *endpoint, 0.f, velocity);
    }

    // Circle might have grazed an endpoint
    if (intersectResult.t < 0 || intersectResult.t > 1)
    {
        sf::Vector2f& nearEndpoint = startB;

        if (intersectResult.t > 1)
        {
            nearEndpoint = endB;
        }

        return circleCircle(centerA, radiusA, nearEndpoint, 0.f, velocity);
    }

    // Circle's velocity intersects line, so it may move through or stop at the line
    sf::Vector2f normal = startB - endB;
    rotate90(normal);

    sf::Vector2f velocityTowardLine = velocity;
    project(velocityTowardLine, normal);

    // We're actually moving away from the line
    if (ECSE::getDotProduct(velocityTowardLine, closeToCircle - centerA) < 0)
    {
        return -1.f;
    }

    // Circle will go this far toward line
    float speedTowardLine = getMagnitude(velocityTowardLine);

    // Circle must go this far toward line
    float distMinusRadius = sqrt(distSqr) - radiusA;

    // Circle will not move close enough to the line
    if (speedTowardLine < distMinusRadius)
    {
        return -1.f;
    }

    return distMinusRadius / speedTowardLine;
}

}
