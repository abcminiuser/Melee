#include "AsteroidGenerator.hpp"

#include <algorithm>

using namespace Melee;

AsteroidGenerator::AsteroidGenerator(Engine& engine, uint32_t generationIntervalMs)
    : m_engine(engine)
    , m_generatorTimer(generationIntervalMs, generationIntervalMs, true)
{
    m_engine.addObserver(this);

    m_currentTotalAsteroids = std::count_if(engine.getEntities().begin(), engine.getEntities().end(), [](const auto& e) { return e->type() == Entity::Type::Asteroid; });
}

AsteroidGenerator::~AsteroidGenerator()
{
    m_engine.removeObserver(this);
}

void AsteroidGenerator::setMaxAsteroids(uint8_t maxAsteroids)
{
    m_maxAsteroids = maxAsteroids;
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

void AsteroidGenerator::updated(Engine& engine, uint32_t msElapsed)
{
    m_generatorTimer.add(msElapsed);

    if (m_generatorTimer.expired() && m_currentTotalAsteroids < m_maxAsteroids)
    {
        AsteroidEntity::AsteroidProperties asteroidProps = {};
        asteroidProps.radius_km = LinearInterpolateRandom(m_minRadius_km, m_maxRadius_km);
        asteroidProps.rotation_degPerSec = (NormalizedRandom() < .5f) ? -120.0f : 120.0f;
        asteroidProps.mass_kg = 1e5;

        const auto newSpeed = LinearInterpolateRandom(m_minVelocity_km_s, m_maxVelocity_km_s);
        const auto startingXYPos = LinearInterpolateRandom(0.0f, engine.getPlayfieldSize());

        const auto position = (NormalizedRandom() < .5f) ? Point{ 0, startingXYPos } : Point{ startingXYPos, 0 };
        const auto velocity = RotationMatrix(LinearInterpolateRandom(0.0f, 360.0f)) * Vector2d { newSpeed, 0 };

        auto asteroid = std::make_shared<AsteroidEntity>(asteroidProps, position, velocity);

        const bool wouldCollide = std::any_of(engine.getEntities().begin(), engine.getEntities().end(), [&](const auto& e){ return engine.checkCollison(asteroid, e); });
        if (!wouldCollide)
            engine.addEntity(asteroid);
    }
}

void AsteroidGenerator::entityAdded(Engine& engine, const std::shared_ptr<Entity>& entity)
{
    if (entity->type() == Entity::Type::Asteroid)
        m_currentTotalAsteroids++;
}

void AsteroidGenerator::entityRemoved(Engine& engine, const std::shared_ptr<Entity>& entity)
{
    if (entity->type() == Entity::Type::Asteroid)
        m_currentTotalAsteroids--;
}
