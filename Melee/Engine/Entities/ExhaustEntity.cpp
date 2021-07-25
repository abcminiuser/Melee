#include "ExhaustEntity.hpp"

#include "Engine/Engine.hpp"

using namespace Melee;


ExhaustEntity::ExhaustEntity(const ExhaustProperties& properties, const Point& pos, const Vector2d& heading)
	: Entity(Entity::Type::Exhaust, properties, pos, heading)
	, m_exhaustProperties(properties)
{

}

void ExhaustEntity::update(Engine& engine, uint32_t msElapsed)
{
	m_age += msElapsed;

	if (m_age >= m_exhaustProperties.maxAge)
		engine.removeEntity(shared_from_this());

	Entity::update(engine, msElapsed);
}
