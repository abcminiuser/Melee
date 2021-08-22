#include "WeaponEntity.hpp"

#include "Engine/Engine.hpp"

using namespace Melee;

namespace
{
    constexpr uint32_t kRotationIntervalMs = 10;
}

WeaponEntity::WeaponEntity(const std::shared_ptr<Entity>& parent,const WeaponProperties& properties, const Point& position, const Vector2d& velocity, const Vector2d& heading)
    : Entity(Entity::Type::Weapon, parent, properties, position)
    , m_visualType(properties.visualType)
    , m_maxAge_ms(properties.maxAge_ms)
    , m_homing(properties.homing)
    , m_linger(properties.linger)
    , m_rotation_degPerSec(properties.rotation_degPerSec)
    , m_rotationTimer(kRotationIntervalMs)
{
    m_heading = heading;
    m_velocity = velocity + (heading * (properties.firingForce_N / properties.mass_kg));

    // Weapons don't increase in velocity once fired, because real physics are no fun.
    m_maxVelocity_km_s = m_velocity.length();

    const float rotationDegreesPerInterval = properties.rotation_degPerSec * kRotationIntervalMs / 1000;
    m_rotationalThrustLeft = RotationMatrix(-rotationDegreesPerInterval);
    m_rotationalThrustRight = RotationMatrix(rotationDegreesPerInterval);
}

void WeaponEntity::update(Engine& engine, uint32_t msElapsed)
{
    m_age += msElapsed;

    if (m_maxAge_ms && m_age >= m_maxAge_ms)
        engine.removeEntity(shared_from_this());

    if (m_homing)
    {
        if (m_lockedTarget.expired())
        {
            auto [distanceSquared, targetEntity] = engine.closestEntity(*this, [this](const Entity& other) { return other.type() == Entity::Type::Ship && other.targetable() && &other != parentEntity().get(); });

            m_lockedTarget = targetEntity;
        }

        m_rotationTimer.add(msElapsed);

        if (const auto target = m_lockedTarget.lock())
        {
            while (m_rotationTimer.expired())
            {
                const auto selfToTarget = (target->position() - m_position);

                const auto selfToTargetRadians = std::atan2(selfToTarget.y, selfToTarget.x);
                const auto currentHeadingRadians = std::atan2(m_heading.y, m_heading.x);

                const auto angleDiffRadians = selfToTargetRadians - currentHeadingRadians;

                if (std::abs(angleDiffRadians) < M_PI)
                    m_heading = ((angleDiffRadians < 0) ? m_rotationalThrustLeft : m_rotationalThrustRight) * m_heading;
                else
                    m_heading = ((angleDiffRadians > 0) ? m_rotationalThrustLeft : m_rotationalThrustRight) * m_heading;
            }
        }

        m_acceleration = m_heading;
    }

    Entity::update(engine, msElapsed);
}

void WeaponEntity::collide(Engine& engine, const std::shared_ptr<Entity>& otherEntity, const PreCollisionState& otherEntityState)
{
    if (otherEntity == m_parentEntity || otherEntity->parentEntity() == m_parentEntity)
        return;

    // Some weapons, like lasers, need to linger even after a collision, they just become no longer collidable.
    if (!m_linger)
        engine.removeEntity(shared_from_this());
    else
        m_collidable = false;

    ExplosionEntity::ExplosionProperties explosionProps = {};
    explosionProps.radius_km = 1000;

    auto explosionEntity = std::make_shared<ExplosionEntity>(nullptr, explosionProps, m_position);
    engine.addEntity(explosionEntity, Engine::InsertionOrder::Bottom);

    Entity::collide(engine, otherEntity, otherEntityState);
}
