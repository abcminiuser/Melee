#include "Entity.hpp"

using namespace Melee;

Entity::Entity(Type type, const Properties& properties, const Point& pos)
	: m_type(type)
	, m_properties(properties)
	, m_position(pos)
{
	m_maxVelocitySquared = properties.maxVelocity * properties.maxVelocity;
}

void Entity::update(Engine& engine, uint32_t msElapsed)
{
	m_position += m_velocity;
	m_velocity += m_acceleration + m_externalAcceleration;

	m_externalAcceleration = {};

	const auto newVelocitySquared = m_velocity.lengthSquared();
	if (newVelocitySquared > m_maxVelocitySquared)
		m_velocity *= m_maxVelocitySquared / newVelocitySquared;
}

void Entity::collide(Engine& engine, Entity& otherEntity)
{

}

void Entity::applyExternalForce(const Vector2d& forceVector)
{
	m_externalAcceleration += forceVector;
}
