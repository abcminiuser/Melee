#include "PlanetEntity.hpp"

#include "Engine/Engine.hpp"

using namespace Melee;

namespace
{
    constexpr auto kGravitationalConstant = 6.674e-11f;
}

PlanetEntity::PlanetEntity(const PlanetProperties& properties, const Point& position)
    : Entity(Entity::Type::Planet, nullptr, properties, position)
    , m_visualType(properties.visualType)
{

}

void PlanetEntity::update(Engine& engine, uint32_t msElapsed)
{
    for (const auto& entity : engine.getEntities())
    {
        if (entity->type() == Entity::Type::Planet)
            continue;

        if (!entity->mass() || entity->position() == m_position)
            continue;

        const auto entityToPlanetVector = (m_position - entity->position());
        const auto gravityForce = (kGravitationalConstant * m_mass_kg * entity->mass()) / entityToPlanetVector.lengthSquared();

        entity->applyExternalForce(entityToPlanetVector.normalised() * (gravityForce / entity->mass()));
    }

    Entity::update(engine, msElapsed);
}
