#include "PlayerEntity.hpp"

#include "Engine/Engine.hpp"

#include <algorithm>

using namespace Melee;

namespace
{
    constexpr uint32_t kRotationIntervalMs = 10;
    constexpr uint32_t kThrustExhaustIntervalMs = 100;
}

PlayerEntity::PlayerEntity(int playerIndex, const PlayerProperties& properties, const Point& position)
    : Entity(Entity::Type::Player, nullptr, properties, position)
    , m_playerIndex(playerIndex)
    , m_playerProperties(properties)
    , m_energyRechargeTimer(properties.energyRechargeRate_ms)
    , m_rotationTimer(kRotationIntervalMs)
    , m_thrustExhaustTimer(kThrustExhaustIntervalMs, kThrustExhaustIntervalMs, true)
    , m_primaryFireTimer(properties.primaryFireRate_ms, properties.primaryFireRate_ms, true)
{
    m_engineAcceleration_ms2    = properties.engineForce_N / properties.mass_kg;

    const float rotationDegreesPerInterval = properties.rotation_degPerSec * kRotationIntervalMs / 1000;
    m_rotationalThrustLeft      = RotationMatrix(-rotationDegreesPerInterval);
    m_rotationalThrustRight     = RotationMatrix(rotationDegreesPerInterval);

    m_health                    = properties.maxHealth;
    m_energy                    = properties.maxEnergy;
}

void PlayerEntity::handleKey(KeyEvent key, bool down)
{
    if (key == KeyEvent::Thrust)
        m_flags.set(Flags::ThrustActive, down);
    else if (key == KeyEvent::ReverseThrust)
        m_flags.set(Flags::ReverseThrustActive, down);
    else if (key == KeyEvent::RotateLeft)
        m_flags.set(Flags::RotateLeftActive, down);
    else if (key == KeyEvent::RotateRight)
        m_flags.set(Flags::RotateRightActive, down);
    else if (key == KeyEvent::FirePrimary)
        m_flags.set(Flags::FirePrimaryActive, down);
    else if (key == KeyEvent::FireSpecial)
        m_flags.set(Flags::FireSpecialActive, down);
}

void PlayerEntity::update(Engine& engine, uint32_t msElapsed)
{
    if (!m_health)
    {
        engine.removeEntity(shared_from_this());

        auto explosionEntity = std::make_shared<ExplosionEntity>(nullptr, ExplosionEntity::ExplosionProperties{}, m_position);
        engine.addEntity(explosionEntity);

        return;
    }

    if (m_flags.test(Flags::RotateLeftActive) || m_flags.test(Flags::RotateRightActive))
    {
        m_rotationTimer.add(msElapsed);

        const auto thrustRotationVector = m_flags.test(Flags::RotateLeftActive) ? m_rotationalThrustLeft : m_rotationalThrustRight;

        while (m_rotationTimer.expired())
            m_heading = thrustRotationVector * m_heading;
    }
    else
    {
        m_rotationTimer.reset();
    }

    if (m_flags.test(Flags::ThrustActive) || m_flags.test(Flags::ReverseThrustActive))
    {
        const auto thrustVector = m_flags.test(Flags::ThrustActive) ? m_heading : -m_heading;

        m_acceleration = thrustVector * m_engineAcceleration_ms2;

        m_thrustExhaustTimer.add(msElapsed);
        if (m_thrustExhaustTimer.expired())
        {
            auto exhaustEntity = std::make_shared<ExhaustEntity>(shared_from_this(), ExhaustEntity::ExhaustProperties{}, m_position, m_velocity + -m_acceleration);
            engine.addEntity(exhaustEntity, Engine::InsertionOrder::Bottom);
        }
    }
    else
    {
        m_acceleration = Vector2d{ 0, 0 };
        m_thrustExhaustTimer.reset();
    }

    m_primaryFireTimer.add(msElapsed);
    if (m_flags.test(Flags::FirePrimaryActive) && m_energy >= m_playerProperties.primaryEnergyCost && m_primaryFireTimer.expired())
    {
        auto projectileEntity = std::make_shared<ProjectileEntity>(shared_from_this(), ProjectileEntity::ProjectileProperties{}, m_position + (m_heading * m_playerProperties.radius_km), m_velocity, m_heading);
        engine.addEntity(projectileEntity);

        consumeEnergy(m_playerProperties.primaryEnergyCost);
    }

    m_energyRechargeTimer.add(msElapsed);
    while (m_energyRechargeTimer.expired())
        consumeEnergy(-1);

    Entity::update(engine, msElapsed);
}

void PlayerEntity::collide(Engine& engine, const std::shared_ptr<Entity>& otherEntity, const PreCollisionState& otherEntityState)
{
    switch (otherEntity->type())
    {
        case Entity::Type::Asteroid:
        case Entity::Type::Planet:
        case Entity::Type::Player:
        {
            m_acceleration = {};
            m_velocity = -m_velocity;

            if (otherEntity->type() == Entity::Type::Planet)
                applyDamage(1);

            break;
        }

        case Entity::Type::Projectile:
        {
            if (otherEntity->parentEntity() != shared_from_this())
            {
                const auto& projectileEntity = std::dynamic_pointer_cast<const ProjectileEntity>(otherEntity);

                applyDamage(projectileEntity->properties().damage);
            }
 
            break;
        }

        case Entity::Type::Exhaust:
        case Entity::Type::Explosion:
            break;
    }

    Entity::collide(engine, otherEntity, otherEntityState);
}

void PlayerEntity::applyDamage(int amount)
{
    m_health = std::clamp<int>(m_health - amount, 0, m_playerProperties.maxHealth);
}

void PlayerEntity::consumeEnergy(int amount)
{
    m_energy = std::clamp<int>(m_energy - amount, 0, m_playerProperties.maxEnergy);
}
