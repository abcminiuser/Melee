#include "Arilou.hpp"

#include "Engine/Engine.hpp"

using namespace Melee;

ArilouShipEntity::ArilouShipEntity(const Point& position)
    : ShipEntity(MakeShipProperties(), position)
{

}

bool ArilouShipEntity::onPrimaryWeaponFired(Engine& engine)
{
    auto [distanceSquared, targetEntity] = engine.closestEntity(*this, [this](const Entity& other) { return (other.type() == Entity::Type::Ship || other.type() == Entity::Type::Asteroid) && &other != parentEntity().get(); });

    constexpr auto kMaxLaserTargetDistance = 8000;
    if (distanceSquared > (kMaxLaserTargetDistance * kMaxLaserTargetDistance))
        distanceSquared = (kMaxLaserTargetDistance * kMaxLaserTargetDistance);

    auto laserTargetPos = m_position + (targetEntity->position() - m_position).normalised() * sqrtf(distanceSquared);

    auto weaponEntity = std::make_shared<ArilouLaserWeaponEntity>(shared_from_this(), laserTargetPos);
    engine.addEntity(weaponEntity);

    return true;
}

bool ArilouShipEntity::onSpecialWeaponFired(Engine& engine)
{
    constexpr auto kMaxTeleportAttempts = 100;

    for (int attempt = 0; attempt < kMaxTeleportAttempts; attempt++)
    {
        const auto startingXPos = LinearInterpolateRandom<float>(m_radius_km, engine.getPlayfieldSize() - m_radius_km);
        const auto startingYPos = LinearInterpolateRandom<float>(m_radius_km, engine.getPlayfieldSize() - m_radius_km);

        const auto position = Point{ startingXPos, startingYPos };

        constexpr auto kMinDistanceAllowedSquared = 100;
        const bool tooClose = std::any_of(engine.getEntities().begin(), engine.getEntities().end(), [&](const auto& e) { return e != shared_from_this() && (e->position() - position).lengthSquared() < (e->radius() + radius() + kMinDistanceAllowedSquared); });
        if (tooClose)
            continue;

        m_position = position;
        break;
    }

    return true;
}

ShipEntity::ShipProperties ArilouShipEntity::MakeShipProperties()
{
    ShipEntity::ShipProperties shipProps = {};

    shipProps.visualType = ShipEntity::VisualType::Arilou;
    shipProps.mass_kg = 1.718e5;
    shipProps.engineForce_N = 10e3;
    shipProps.rotation_degPerSec = 360;
    shipProps.maxVelocity_km_s = 10;
    shipProps.maxHealth = 6;
    shipProps.maxEnergy = 20;
    shipProps.radius_km = 600;
    shipProps.primaryEnergyCost = 2;
    shipProps.specialEnergyCost = 3;
    shipProps.primaryFireRate_ms = 150;
    shipProps.affectedByGravity = false;

    return shipProps;
}

void ArilouShipEntity::onControlFlagsUpdated()
{
    if (!m_controlFlags.test(ControlFlags::ThrustActive) && !m_controlFlags.test(ControlFlags::ReverseThrustActive))
        m_velocity = {};

    ShipEntity::onControlFlagsUpdated();
}

// --

ArilouLaserWeaponEntity::ArilouLaserWeaponEntity(const std::shared_ptr<Entity>& parent, const Point& position)
    : WeaponEntity(parent, MakeWeaponProperties(), position)
{

}

WeaponEntity::WeaponProperties ArilouLaserWeaponEntity::MakeWeaponProperties()
{
    WeaponEntity::WeaponProperties weaponProps = {};

    weaponProps.visualType = WeaponEntity::VisualType::LaserYellow;
    weaponProps.collisionDamage = 1;
    weaponProps.maxAge_ms = 100;
    weaponProps.linger = true;

    return weaponProps;
}
