#include "AsteroidEntity.hpp"

#include "Engine/Engine.hpp"

using namespace Melee;

namespace
{
    constexpr uint32_t kRotationIntervalMs = 10;
}

AsteroidEntity::AsteroidEntity(const AsteroidProperties& properties, const Point& pos, const Vector2d& heading)
    : Entity(Entity::Type::Asteroid, properties, pos, heading)
    , m_asteroidProperties(properties)
    , m_rotationTimer(kRotationIntervalMs)
{
    const float rotationDegreesPerInterval = properties.rotation_degPerSec * kRotationIntervalMs / 1000;
    m_rotation = RotationMatrix(rotationDegreesPerInterval);
}

void AsteroidEntity::update(Engine& engine, uint32_t msElapsed)
{
    m_rotationTimer.add(msElapsed);

    while (m_rotationTimer.expired())
        m_heading = m_rotation * m_heading;

    Entity::update(engine, msElapsed);
}

void AsteroidEntity::collide(Engine& engine, const Entity& otherEntity)
{
    switch (otherEntity.type())
    {
        case Entity::Type::Asteroid:
        case Entity::Type::Planet:
        case Entity::Type::Player:
        {
            m_acceleration = {};
            m_velocity = -m_velocity;

            break;
        }
    }

    Entity::collide(engine, otherEntity);
}
