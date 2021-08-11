#include "WeaponEntity.hpp"

#include "Engine/Engine.hpp"

using namespace Melee;

WeaponEntity::WeaponEntity(const std::shared_ptr<Entity>& parent,const WeaponProperties& properties, const Point& position, const Vector2d& velocity, const Vector2d& heading)
    : Entity(Entity::Type::Weapon, parent, properties, position)
    , m_visualType(properties.visualType)
    , m_maxAge_ms(properties.maxAge_ms)
    , m_damage(properties.damage)
{
    m_heading = heading;
    m_velocity = velocity + (heading * (properties.firingForce_N / properties.mass_kg));
}

void WeaponEntity::update(Engine& engine, uint32_t msElapsed)
{
    m_age += msElapsed;

    if (m_maxAge_ms && m_age >= m_maxAge_ms)
        engine.removeEntity(shared_from_this());

    Entity::update(engine, msElapsed);
}

void WeaponEntity::collide(Engine& engine, const std::shared_ptr<Entity>& otherEntity, const PreCollisionState& otherEntityState)
{
    if (otherEntity == m_parentEntity || otherEntity->parentEntity() == m_parentEntity)
        return;

    engine.removeEntity(shared_from_this());

    ExplosionEntity::ExplosionProperties explosionProps = {};
    explosionProps.radius_km = 1000;

    auto explosionEntity = std::make_shared<ExplosionEntity>(nullptr, explosionProps, m_position);
    engine.addEntity(explosionEntity, Engine::InsertionOrder::Bottom);

    Entity::collide(engine, otherEntity, otherEntityState);
}
