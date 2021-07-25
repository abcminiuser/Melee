#include "PlayerEntity.hpp"

#include "Engine/Engine.hpp"

#include <algorithm>

using namespace Melee;

namespace
{
    constexpr uint32_t kRotationIntervalMs = 10;
    constexpr uint32_t kThrustExhaustIntervalMs = 100;
}

PlayerEntity::PlayerEntity(int playerIndex, const PlayerProperties& properties, const Point& pos)
    : Entity(Entity::Type::Player, properties, pos)
    , m_playerIndex(playerIndex)
    , m_playerProperties(properties)
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
    const auto UpdateFlag = [&](Flags::Mask flag, bool state)
        {
            if (state)
                m_flags |= flag;
            else
                m_flags &= ~flag;
        };

    if (key == KeyEvent::Thrust)
        UpdateFlag(Flags::ThrustActive, down);
    else if (key == KeyEvent::ReverseThrust)
        UpdateFlag(Flags::ReverseThrustActive, down);
    else if (key == KeyEvent::RotateLeft)
        UpdateFlag(Flags::RotateLeftActive, down);
    else if (key == KeyEvent::RotateRight)
        UpdateFlag(Flags::RotateRightActive, down);
    else if (key == KeyEvent::FirePrimary)
        UpdateFlag(Flags::FirePrimaryActive, down);
    else if (key == KeyEvent::FireSpecial)
        UpdateFlag(Flags::FireSpecialActive, down);
}

void PlayerEntity::update(Engine& engine, uint32_t msElapsed)
{
    if (!m_health)
    {
        engine.removeEntity(shared_from_this());

        auto explosionEntity = std::make_shared<ExplosionEntity>(ExplosionEntity::ExplosionProperties{}, m_position);
        engine.addEntity(explosionEntity);

        return;
    }

    const auto rotateFlags = m_flags & (Flags::RotateLeftActive | Flags::RotateRightActive);
    if (rotateFlags == Flags::RotateLeftActive)
    {
        m_rotationMsElapsed += msElapsed;

        while (m_rotationMsElapsed > kRotationIntervalMs)
        {
            m_heading = m_rotationalThrustLeft * m_heading;
            m_rotationMsElapsed -= std::min(kRotationIntervalMs, m_rotationMsElapsed);
        }
    }
    else if (rotateFlags == Flags::RotateRightActive)
    {
        m_rotationMsElapsed += msElapsed;

        while (m_rotationMsElapsed >= kRotationIntervalMs)
        {
            m_heading = m_rotationalThrustRight * m_heading;
            m_rotationMsElapsed -= std::min(kRotationIntervalMs, m_rotationMsElapsed);
        }
    }
    else
    {
        m_rotationMsElapsed = 0;
    }

    const auto thrustFlags = m_flags & (Flags::ThrustActive | Flags::ReverseThrustActive);
    if (thrustFlags == Flags::ThrustActive || thrustFlags == Flags::ReverseThrustActive)
    {
        m_thrustExhaustMsElapsed += msElapsed;

        const auto thrustVector = m_heading * ((thrustFlags == Flags::ThrustActive) ? 1 : -1);

        m_acceleration = thrustVector * m_engineAcceleration_ms2;

        if (m_thrustExhaustMsElapsed >= kThrustExhaustIntervalMs)
        {
            auto exhaustEntity = std::make_shared<ExhaustEntity>(ExhaustEntity::ExhaustProperties{}, m_position, -m_acceleration);
            engine.addEntity(exhaustEntity);

            m_thrustExhaustMsElapsed = 0;
        }
    }
    else
    {
        m_acceleration = Vector2d{ 0, 0 };
        m_thrustExhaustMsElapsed = 0;
    }

    if (m_primaryFireMsElapsed >= m_playerProperties.primaryFireRate_ms)
    {
        if (m_flags & Flags::FirePrimaryActive && m_energy >= m_playerProperties.primaryEnergyCost)
        {
            auto projectileEntity = std::make_shared<ProjectileEntity>(ProjectileEntity::ProjectileProperties{}, shared_from_this(), m_position + m_heading * (m_playerProperties.radius_km + 500), m_heading * 30);
            engine.addEntity(projectileEntity);

            consumeEnergy(m_playerProperties.primaryEnergyCost);

            m_primaryFireMsElapsed = 0;
        }
    }
    else
    {
        m_primaryFireMsElapsed += msElapsed;
    }

    if (m_energyRechargeElapsed > m_playerProperties.energyRechargeRate_ms)
    {
        consumeEnergy(-1);
        m_energyRechargeElapsed -= m_playerProperties.energyRechargeRate_ms;
    }
    else
    {
        m_energyRechargeElapsed += msElapsed;
    }

    Entity::update(engine, msElapsed);
}

void PlayerEntity::collide(Engine& engine, const Entity& otherEntity)
{
    switch (otherEntity.type())
    {
        case Entity::Type::Asteroid:
        case Entity::Type::Planet:
        case Entity::Type::Player:
        {
            m_acceleration = {};
            m_velocity = -m_velocity;

            if (otherEntity.type() == Entity::Type::Planet)
                applyDamage(1);

            break;
        }

        case Entity::Type::Projectile:
        {
            const auto& projectileEntity = *dynamic_cast<const ProjectileEntity*>(&otherEntity);

            if (projectileEntity.ownerEntity() != shared_from_this())
                applyDamage(projectileEntity.properties().damage);

            break;
        }
    }

    Entity::collide(engine, otherEntity);
}

void PlayerEntity::applyDamage(int amount)
{
    m_health = std::clamp<int>(m_health - amount, 0, m_playerProperties.maxHealth);
}

void PlayerEntity::consumeEnergy(int amount)
{
    m_energy = std::clamp<int>(m_energy - amount, 0, m_playerProperties.maxEnergy);
}
