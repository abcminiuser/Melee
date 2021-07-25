#include "PlanetEntity.hpp"

#include "Engine/Engine.hpp"

using namespace Melee;

namespace
{
	constexpr auto kGravitationalConstant = 6.674e-11;
}

PlanetEntity::PlanetEntity(const PlanetProperties& properties, const Point& pos)
	: Entity(Entity::Type::Planet, properties, pos)
	, m_planetProperties(properties)
{

}

void PlanetEntity::update(Engine& engine, uint32_t msElapsed)
{
	for (const auto& entity : engine.getEntities(Entity::Type::Player))
	{
		const auto entityToPlanetVector		= (m_position - entity->position());

		const auto distanceToPlanetSquared	= entityToPlanetVector.lengthSquared();
		if (distanceToPlanetSquared == 0)
			continue;

		const auto gravityForce = (kGravitationalConstant * m_planetProperties.mass_kg * entity->properties().mass_kg) / distanceToPlanetSquared;
		entity->applyExternalForce(entityToPlanetVector.normalised() * (gravityForce / entity->properties().mass_kg));
	}

	Entity::update(engine, msElapsed);
}
