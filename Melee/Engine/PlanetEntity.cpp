#include "PlanetEntity.hpp"

using namespace Melee;

PlanetEntity::PlanetEntity(const PlanetProperties& properties, const Point& pos)
	: Entity(Entity::Type::Planet, properties, pos)
	, m_planetProperties(properties)
{

}

void PlanetEntity::update(const EntityList& entities, uint32_t msElapsed)
{
	for (const auto& entity : entities)
	{
		if (entity->type() != Entity::Type::Player)
			continue;

		const auto entityToPlanetVector		= (m_position - entity->position());

		const auto distanceToPlanetSquared	= entityToPlanetVector.lengthSquared();
		if (distanceToPlanetSquared == 0)
			continue;

		const auto gravityForce = 6.674e-11 * m_planetProperties.mass_kg * entity->mass() / distanceToPlanetSquared;
		entity->applyExternalForce((entityToPlanetVector / sqrtf(distanceToPlanetSquared)) * (gravityForce / entity->mass()));
	}

	Entity::update(entities, msElapsed);
}
