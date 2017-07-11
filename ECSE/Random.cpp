#include "Random.h"
#include "VectorMath.h"

namespace ECSE
{

std::random_device randomDevice;
std::mt19937 ECSE::randomEngine(randomDevice());

sf::Vector2f ECSE::randomSpreadVector(float midAngle, float angleSpread, float minMag, float maxMag)
{
    auto angleDist = std::uniform_real_distribution<float>(
        midAngle - angleSpread * 0.5f,
        midAngle + angleSpread * 0.5f
        );
    auto magDist = std::uniform_real_distribution<float>(minMag, maxMag);

    float angle = angleDist(randomEngine);
    float mag = magDist(randomEngine);

    auto v = sf::Vector2f(mag, 0.f);
    setHeading(v, angle);

    return v;
}

}