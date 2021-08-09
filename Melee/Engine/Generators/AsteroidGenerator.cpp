#include "AsteroidGenerator.hpp"

#include <algorithm>

using namespace Melee;

namespace
{
    constexpr auto kMaxGeneratorAttempts = 100;
}

AsteroidGenerator::AsteroidGenerator(Engine& engine)
    : EntityGenerator(engine, Entity::Type::Asteroid)
{

}

void AsteroidGenerator::setVelocityRange(float minVelocity, float maxVelocity)
{
    m_minVelocity_km_s = minVelocity;
    m_maxVelocity_km_s = maxVelocity;
}

void AsteroidGenerator::setRadiusRange(float minRadius, float maxRadius)
{
    m_minRadius_km = minRadius;
    m_maxRadius_km = maxRadius;
}

void AsteroidGenerator::generate()
{
    AsteroidEntity::AsteroidProperties asteroidProps = {};
    asteroidProps.radius_km = LinearInterpolateRandom(m_minRadius_km, m_maxRadius_km);
    asteroidProps.rotation_degPerSec = (NormalizedRandom() < .5f) ? -120.0f : 120.0f;
    asteroidProps.mass_kg = 1e5;

    const auto newSpeed = LinearInterpolateRandom(m_minVelocity_km_s, m_maxVelocity_km_s);
    const auto velocity = RotationMatrix(LinearInterpolateRandom(0.0f, 360.0f)) * Vector2d { newSpeed, 0 };

    for (int attempt = 0; attempt < kMaxGeneratorAttempts; attempt++)
    {
        const auto startingXYPos = LinearInterpolateRandom(0.0f, m_engine.getPlayfieldSize());
        const auto position = (NormalizedRandom() < .5f) ? Point{ 0, startingXYPos } : Point{ startingXYPos, 0 };

        auto asteroid = std::make_shared<AsteroidEntity>(asteroidProps, position, velocity);

        const bool wouldCollide = std::any_of(m_engine.getEntities().begin(), m_engine.getEntities().end(), [&](const auto& e){ return m_engine.checkCollison(asteroid, e); });
        if (wouldCollide)
            continue;

        m_engine.addEntity(asteroid);
        break;
    }
}
