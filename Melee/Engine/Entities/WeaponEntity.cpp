#include "WeaponEntity.hpp"

#include "Engine/Engine.hpp"

using namespace Melee;

WeaponEntity::WeaponEntity(const std::shared_ptr<Entity>& parent,const WeaponProperties& properties, const Point& position, const Vector2d& velocity, const Vector2d& heading)
    : Entity(Entity::Type::Weapon, parent, properties, position)
    , m_weaponProperties(properties)
{
    m_heading = heading;
    m_velocity = velocity + (heading * (properties.firingForce_N / properties.mass_kg));
}

void WeaponEntity::update(Engine& engine, uint32_t msElapsed)
{
    m_age += msElapsed;

    if (m_age >= m_weaponProperties.maxAge_ms)
        engine.removeEntity(shared_from_this());

    Entity::update(engine, msElapsed);
}

void WeaponEntity::collide(Engine& engine, const std::shared_ptr<Entity>& otherEntity, const PreCollisionState& otherEntityState)
{
    if (otherEntity == m_parentEntity)
        return;

    engine.removeEntity(shared_from_this());

    ExplosionEntity::ExplosionProperties explosionProps = {};
    explosionProps.radius_km = 1000;

    auto explosionEntity = std::make_shared<ExplosionEntity>(nullptr, explosionProps, m_position);
    engine.addEntity(explosionEntity);

    Entity::collide(engine, otherEntity, otherEntityState);
}