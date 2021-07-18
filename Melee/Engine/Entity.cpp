#include "Entity.hpp"

#include <cstdio>

using namespace Melee;

Entity::Entity(Type type, const Point& pos)
	: m_type(type)
	, m_position(pos)
	, m_heading{ 1, 0 }
{

}

void Entity::update(uint32_t msElapsed)
{
	m_position += m_velocity;

	if (! m_maxVelocitySquared || ((m_velocity.x * m_velocity.x + m_velocity.y * m_velocity.y) < m_maxVelocitySquared))
		m_velocity += m_acceleration;
}

void Entity::setMaxVelocity(uint32_t velocity)
{
	m_maxVelocitySquared = velocity * velocity;
}
