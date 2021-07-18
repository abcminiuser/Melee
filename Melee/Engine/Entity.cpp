#include "Entity.hpp"

#include <cstdio>

using namespace Melee;

Entity::Entity(Type type, const Properties& properties, const Point& pos)
	: m_type(type)
	, m_properties(properties)
	, m_position(pos)
{
	m_maxVelocitySquared = properties.maxVelocity * properties.maxVelocity;
}

void Entity::update(uint32_t msElapsed)
{
	m_position += m_velocity;
	m_velocity += m_acceleration;

	const auto newVelocitySquared = m_velocity.lengthSquared();
	if (newVelocitySquared > m_maxVelocitySquared)
		m_velocity *= m_maxVelocitySquared / newVelocitySquared;
}