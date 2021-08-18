#include "PlanetGenerator.hpp"

#include <algorithm>

using namespace Melee;

namespace
{
    constexpr auto kMaxGeneratorAttempts = 100;

    constexpr PlanetEntity::VisualType kPlanetVisualTypes[] =
        {
			PlanetEntity::VisualType::Acid,
			PlanetEntity::VisualType::Alkali,
			PlanetEntity::VisualType::Auric,
			PlanetEntity::VisualType::Azure,
			PlanetEntity::VisualType::Bluegas,
			PlanetEntity::VisualType::Carbide,
			PlanetEntity::VisualType::Chlorine,
			PlanetEntity::VisualType::Chondrite,
			PlanetEntity::VisualType::Cimmerian,
			PlanetEntity::VisualType::Copper,
			PlanetEntity::VisualType::Crimson,
			PlanetEntity::VisualType::Cyangas,
			PlanetEntity::VisualType::Cyanic,
			PlanetEntity::VisualType::Dust,
			PlanetEntity::VisualType::Emerald,
			PlanetEntity::VisualType::Fluorescent,
			PlanetEntity::VisualType::Green,
			PlanetEntity::VisualType::Greengas,
			PlanetEntity::VisualType::Greygas,
			PlanetEntity::VisualType::Halide,
			PlanetEntity::VisualType::Hydrocarbon,
			PlanetEntity::VisualType::Infrared,
			PlanetEntity::VisualType::Iodine,
			PlanetEntity::VisualType::Lanthanide,
			PlanetEntity::VisualType::Magma,
			PlanetEntity::VisualType::Magnetic,
			PlanetEntity::VisualType::Maroon,
			PlanetEntity::VisualType::Metal,
			PlanetEntity::VisualType::Noble,
			PlanetEntity::VisualType::Oolite,
			PlanetEntity::VisualType::Opalescent,
			PlanetEntity::VisualType::Organic,
			PlanetEntity::VisualType::Pellucid,
			PlanetEntity::VisualType::Plutonic,
			PlanetEntity::VisualType::Primordial,
			PlanetEntity::VisualType::Purple,
			PlanetEntity::VisualType::Purplegas,
			PlanetEntity::VisualType::Quasidegenerate,
			PlanetEntity::VisualType::Radioactive,
			PlanetEntity::VisualType::Rainbow,
			PlanetEntity::VisualType::Redgas,
			PlanetEntity::VisualType::Redux,
			PlanetEntity::VisualType::Ruby,
			PlanetEntity::VisualType::Sapphire,
			PlanetEntity::VisualType::Selenic,
			PlanetEntity::VisualType::Shattered,
			PlanetEntity::VisualType::Slaveshield,
			PlanetEntity::VisualType::Superdense,
			PlanetEntity::VisualType::Telluric,
			PlanetEntity::VisualType::Treasure,
			PlanetEntity::VisualType::Ultramarine,
			PlanetEntity::VisualType::Urea,
			PlanetEntity::VisualType::Vinylogous,
			PlanetEntity::VisualType::Violetgas,
			PlanetEntity::VisualType::Water,
			PlanetEntity::VisualType::Xenolithic,
			PlanetEntity::VisualType::Yellowgas,
			PlanetEntity::VisualType::Yttric,
        };
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
    planetProps.visualType = kPlanetVisualTypes[LinearInterpolateIndex(kPlanetVisualTypes, NormalizedRandom())];

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
