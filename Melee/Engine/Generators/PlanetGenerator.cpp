#include "PlanetGenerator.hpp"

#include <algorithm>

using namespace Melee;

namespace
{
    constexpr auto kMaxGeneratorAttempts = 100;
}

PlanetGenerator::PlanetGenerator(Engine& engine)
    : EntityGenerator(engine, Entity::Type::Planet)
{

}

void PlanetGenerator::setMassRange(float minMass, float maxMass)
{
    m_minMass_kg = minMass;
    m_maxMass_kg = maxMass;
}

void PlanetGenerator::setRadiusRange(float minRadius, float maxRadius)
{
    m_minRadius_km = minRadius;
    m_maxRadius_km = maxRadius;
}

void PlanetGenerator::setMinimumDistanceMultiplier(uint8_t multiplier)
{
    m_minDistanceMultiplier = std::clamp<uint8_t>(multiplier, 5, 20);
}

void PlanetGenerator::generate()
{
    PlanetEntity::PlanetProperties planetProps = {};
    planetProps.radius_km = LinearInterpolateRandom(m_minRadius_km, m_maxRadius_km);
    planetProps.mass_kg = LinearInterpolateRandom(m_minMass_kg, m_maxMass_kg);

    const auto minDistanceAllowedSquared = (planetProps.radius_km * planetProps.radius_km) * (m_minDistanceMultiplier * m_minDistanceMultiplier);

    for (int attempt = 0; attempt < kMaxGeneratorAttempts; attempt++)
    {
        const auto startingXPos = LinearInterpolateRandom<float>(planetProps.radius_km, m_engine.getPlayfieldSize() - planetProps.radius_km);
        const auto startingYPos = LinearInterpolateRandom<float>(planetProps.radius_km, m_engine.getPlayfieldSize() - planetProps.radius_km);

        const auto position = Point{ startingXPos, startingYPos };

        auto planet = std::make_shared<PlanetEntity>(planetProps, position);

        const bool tooClose = std::any_of(m_engine.getEntities().begin(), m_engine.getEntities().end(), [&](const auto& e) { return (e->position() - position).lengthSquared() < minDistanceAllowedSquared; });
        if (tooClose)
            continue;

        m_engine.addEntity(planet);
        break;
    }
}
