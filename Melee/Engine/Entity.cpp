#include "Entity.hpp"
#include "Engine.hpp"

using namespace Melee;

Entity::Entity(Type type, const std::shared_ptr<Entity>& parent, const Properties& properties, const Point& pos)
    : m_type(type)
    , m_parentEntity(parent)
    , m_mass_kg(properties.mass_kg)
    , m_radius_km(properties.radius_km)
    , m_maxVelocity_km_s(properties.maxVelocity_km_s)
    , m_collidable(properties.collidable)
    , m_wrappable(properties.wrappable)
    , m_targetable(properties.targetable)
    , m_affectedByGravity(properties.affectedByGravity)
    , m_collisionDamage(properties.collisionDamage)
    , m_position(pos)
{

}

void Entity::update(Engine& engine, uint32_t msElapsed)
{
    m_position += m_velocity * static_cast<float>(msElapsed);
    m_velocity += (m_acceleration + m_externalAcceleration) * static_cast<float>(msElapsed);

    m_externalAcceleration = {};

    if (const auto maxVelocitySquared = m_maxVelocity_km_s * m_maxVelocity_km_s)
    {
        const auto newVelocitySquared = m_velocity.lengthSquared();
        if (newVelocitySquared > maxVelocitySquared)
            m_velocity *= maxVelocitySquared / newVelocitySquared;
    }

    if (m_wrappable)
    {
        const auto maxPos = engine.getPlayfieldSize();

        if (m_position.x < 0)
            m_position.x += maxPos;
        else if (m_position.x > maxPos)
            m_position.x -= maxPos;

        if (m_position.y < 0)
            m_position.y += maxPos;
        else if (m_position.y > maxPos)
            m_position.y -= maxPos;
    }
}

void Entity::collide(Engine& engine, const std::shared_ptr<Entity>& otherEntity, const PreCollisionState& otherEntityState)
{

}

void Entity::moveTo(const Vector2d& newPosition)
{
    m_position = newPosition;
}

void Entity::applyExternalForce(const Vector2d& forceVector)
{
    m_externalAcceleration += forceVector;
}
