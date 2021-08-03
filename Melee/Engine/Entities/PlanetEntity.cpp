#include "PlanetEntity.hpp"

#include "Engine/Engine.hpp"

using namespace Melee;

namespace
{
    constexpr auto kGravitationalConstant = 6.674e-11f;
}

PlanetEntity::PlanetEntity(const PlanetProperties& properties, const Point& position)
    : Entity(Entity::Type::Planet, nullptr, properties, position)
    , m_planetProperties(properties)
{

}

void PlanetEntity::update(Engine& engine, uint32_t msElapsed)
{
    for (const auto& entity : engine.getEntities())
    {
        if (entity->type() != Entity::Type::Player)
            continue;

        const auto entityToPlanetVector     = (m_position - entity->position());

        const auto distanceToPlanetSquared  = entityToPlanetVector.lengthSquared();
        if (distanceToPlanetSquared == 0)
            continue;

        const auto gravityForce = (kGravitationalConstant * m_planetProperties.mass_kg * entity->properties().mass_kg) / distanceToPlanetSquared;
        entity->applyExternalForce(entityToPlanetVector.normalised() * (gravityForce / entity->properties().mass_kg));
    }

    Entity::update(engine, msElapsed);
}
