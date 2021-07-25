#include "Entity.hpp"

using namespace Melee;

Entity::Entity(Type type, const Properties& properties, const Point& pos, const Point& vel, const Point& acc)
    : m_type(type)
    , m_properties(properties)
    , m_position(pos)
    , m_velocity(vel)
    , m_acceleration(acc)
{
    m_maxVelocitySquared = properties.maxVelocity * properties.maxVelocity;
}

void Entity::update(Engine& engine, uint32_t msElapsed)
{
    m_position += m_velocity * msElapsed;
    m_velocity += (m_acceleration + m_externalAcceleration) * msElapsed;

    m_externalAcceleration = {};

    if (m_maxVelocitySquared)
    {
        const auto newVelocitySquared = m_velocity.lengthSquared();
        if (newVelocitySquared > m_maxVelocitySquared)
            m_velocity *= m_maxVelocitySquared / newVelocitySquared;
    }
}

void Entity::collide(Engine& engine, const Entity& otherEntity, const PreCollisionState& otherEntityState)
{

}

void Entity::applyExternalForce(const Vector2d& forceVector)
{
    m_externalAcceleration += forceVector;
}
