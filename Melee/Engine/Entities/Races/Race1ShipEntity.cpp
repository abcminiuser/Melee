#include "Race1ShipEntity.hpp"

#include "Engine/Engine.hpp"

using namespace Melee;

namespace
{
    constexpr auto kMaxMines = 5;
}

Race1ShipEntity::Race1ShipEntity(const Point& position)
    : ShipEntity(MakeShipProperties(), position)
{

}

void Race1ShipEntity::onPrimaryWeaponFired(Engine& engine)
{
	const auto spawnLocation = m_position + (m_heading * m_radius_km);

    WeaponEntity::WeaponProperties weaponProps = {};

    weaponProps.visualType = WeaponEntity::VisualType::Race1Missile;

    auto weaponEntity = std::make_shared<WeaponEntity>(shared_from_this(), weaponProps, spawnLocation, m_velocity, m_heading);
    engine.addEntity(weaponEntity);
}

void Race1ShipEntity::onSpecialWeaponFired(Engine& engine)
{
    const auto spawnLocation = m_position + (-m_heading * m_radius_km);

    auto weaponEntity = std::make_shared<WeaponEntity>(shared_from_this(), MakeSpecialWeaponProperties(), spawnLocation);
    engine.addEntity(weaponEntity, Engine::InsertionOrder::Bottom);

    m_mines.push_back(weaponEntity);
    m_mines.remove_if([](const auto& m) { return m.expired(); });
    if (m_mines.size() > kMaxMines)
    {
        engine.removeEntity(m_mines.front().lock());
        m_mines.pop_front();
    }
}

ShipEntity::ShipProperties Race1ShipEntity::MakeShipProperties()
{
    ShipEntity::ShipProperties shipProps = {};

    shipProps.visualType = ShipEntity::VisualType::Race1Ship;
    shipProps.mass_kg = 9.718e5;
    shipProps.engineForce_N = 8e3;
    shipProps.rotation_degPerSec = 100;
    shipProps.maxVelocity_km_s = 40;
    shipProps.maxHealth = 2;
    shipProps.maxEnergy = 10;
    shipProps.radius_km = 1000;

    return shipProps;
}

WeaponEntity::WeaponProperties Race1ShipEntity::MakePrimaryWeaponProperties()
{
    WeaponEntity::WeaponProperties weaponProps = {};

    weaponProps.visualType = WeaponEntity::VisualType::Race1Missile;

    return weaponProps;
}

WeaponEntity::WeaponProperties Race1ShipEntity::MakeSpecialWeaponProperties()
{
    WeaponEntity::WeaponProperties weaponProps = {};

    weaponProps.visualType = WeaponEntity::VisualType::Race1Mine;
    weaponProps.radius_km = 800;
    weaponProps.mass_kg = 1000;
    weaponProps.maxAge_ms = 0;
    weaponProps.damage = 5;

    return weaponProps;
}
