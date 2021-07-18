#include "PlanetEntity.hpp"

using namespace Melee;

PlanetEntity::PlanetEntity(const PlanetProperties& properties, const Point& pos)
	: Entity(Entity::Type::Planet, properties, pos)
	, m_planetProperties(properties)
{

}
