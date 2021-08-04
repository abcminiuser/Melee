#include "AsteroidGenerator.hpp"

#include <algorithm>

using namespace Melee;

AsteroidGenerator::AsteroidGenerator(Engine& engine, uint8_t maxAsteroids, float minVelocity_km_s, float maxVelocity_km_s, float minRadius_km, float maxRadius_km, uint32_t generationIntervalMs)
    : m_engine(engine)
    , m_maxAsteroids(maxAsteroids)
    , m_minVelocity_km_s(minVelocity_km_s)
    , m_maxVelocity_km_s(maxVelocity_km_s)
    , m_minRadius_km(minRadius_km)
    , m_maxRadius_km(maxRadius_km)
    , m_generatorTimer(generationIntervalMs, generationIntervalMs, true)
    , m_currentTotalAsteroids(std::count_if(engine.getEntities().begin(), engine.getEntities().end(), [](const auto& e) { return e->type() == Entity::Type::Asteroid; }))
{
    m_engine.addObserver(this);
}

AsteroidGenerator::~AsteroidGenerator()
{
    m_engine.removeObserver(this);
}

void AsteroidGenerator::updated(Engine& engine, uint32_t msElapsed)
{
    m_generatorTimer.add(msElapsed);

    if (m_currentTotalAsteroids < m_maxAsteroids && m_generatorTimer.expired())
    {
        AsteroidEntity::AsteroidProperties asteroidProps = {};
        asteroidProps.radius_km = LinearInterpolateRandom(m_minRadius_km, m_maxRadius_km);
        asteroidProps.rotation_degPerSec = (NormalizedRandom() < .5f) ? -120.0f : 120.0f;
        asteroidProps.mass_kg = 1e5;

        const auto newSpeed = LinearInterpolateRandom(m_minVelocity_km_s, m_maxVelocity_km_s);
        const auto startingXYPos = LinearInterpolateRandom(0.0f, engine.getPlayfieldSize());

        const auto velocity = RotationMatrix(LinearInterpolateRandom(0.0f, 360.0f)) * Vector2d { newSpeed, 0 };
        const auto position = (NormalizedRandom() < .5f) ? Point{ 0, startingXYPos } : Point{ startingXYPos, 0 };

        auto asteroid = std::make_shared<AsteroidEntity>(asteroidProps, position, velocity);

        const auto entities = engine.getEntities();
        const bool wouldCollide = std::any_of(entities.begin(), entities.end(), [&](const auto& e){ return engine.checkCollison(asteroid, e); });

        if (!wouldCollide)
        {
            engine.addEntity(asteroid);

            m_currentTotalAsteroids++;
        }
    }
}

void AsteroidGenerator::entityRemoved(Engine& engine, const std::shared_ptr<Entity>& entity)
{
    if (entity->type() == Entity::Type::Asteroid)
        m_currentTotalAsteroids--;
}
