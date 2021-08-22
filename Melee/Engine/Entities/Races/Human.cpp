#include "Human.hpp"

#include "Engine/Engine.hpp"

using namespace Melee;

HumanShipEntity::HumanShipEntity(const Point& position)
    : ShipEntity(MakeShipProperties(), position)
{

}

bool HumanShipEntity::onPrimaryWeaponFired(Engine& engine)
{
    const auto spawnLocation = m_position + (m_heading * m_radius_km);

    auto weaponEntity = std::make_shared<HumanMissileWeaponEntity>(shared_from_this(), spawnLocation, m_velocity, m_heading);
    engine.addEntity(weaponEntity);

    return true;
}

bool HumanShipEntity::onSpecialWeaponFired(Engine& engine)
{
    auto [distanceSquared, targetEntity] = engine.closestEntity(*this, [this](const Entity& other) { return (other.type() == Entity::Type::Ship || other.type() == Entity::Type::Asteroid) && &other != parentEntity().get(); });

    constexpr auto kMaxLaserTargetDistance = 5000;
    if (distanceSquared > (kMaxLaserTargetDistance * kMaxLaserTargetDistance))
        return false;

    auto weaponEntity = std::make_shared<HumanLaserWeaponEntity>(shared_from_this(), targetEntity->position());
    engine.addEntity(weaponEntity);

    return true;
}

ShipEntity::ShipProperties HumanShipEntity::MakeShipProperties()
{
    ShipEntity::ShipProperties shipProps = {};

    shipProps.visualType = ShipEntity::VisualType::Human;
    shipProps.mass_kg = 9.718e5;
    shipProps.engineForce_N = 8e3;
    shipProps.rotation_degPerSec = 100;
    shipProps.maxVelocity_km_s = 24;
    shipProps.maxHealth = 18;
    shipProps.maxEnergy = 18;
    shipProps.radius_km = 1000;
    shipProps.primaryEnergyCost = 9;
    shipProps.specialEnergyCost = 4;
    shipProps.primaryFireRate_ms = 3000;
    shipProps.specialFireRate_ms = 300;

    return shipProps;
}

// --

HumanMissileWeaponEntity::HumanMissileWeaponEntity(const std::shared_ptr<Entity>& parent, const Point& position, const Vector2d& velocity, const Vector2d& heading)
    : WeaponEntity(parent, MakeWeaponProperties(), position, velocity, heading)
{

}

WeaponEntity::WeaponProperties HumanMissileWeaponEntity::MakeWeaponProperties()
{
    WeaponEntity::WeaponProperties weaponProps = {};

    weaponProps.visualType = WeaponEntity::VisualType::HumanMissile;
    weaponProps.collisionDamage = 4;
    weaponProps.homing = true;
    weaponProps.rotation_degPerSec = 120;
    weaponProps.maxAge_ms = 8000;

    return weaponProps;
}

// --

HumanLaserWeaponEntity::HumanLaserWeaponEntity(const std::shared_ptr<Entity>& parent, const Point& position)
    : WeaponEntity(parent, MakeWeaponProperties(), position)
{

}

WeaponEntity::WeaponProperties HumanLaserWeaponEntity::MakeWeaponProperties()
{
    WeaponEntity::WeaponProperties weaponProps = {};

    weaponProps.visualType = WeaponEntity::VisualType::LaserWhite;
    weaponProps.collisionDamage = 1;
    weaponProps.maxAge_ms = 100;
    weaponProps.linger = true;

    return weaponProps;
}
