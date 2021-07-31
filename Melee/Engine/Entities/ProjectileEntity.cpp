#include "ProjectileEntity.hpp"

#include "Engine/Engine.hpp"

using namespace Melee;

ProjectileEntity::ProjectileEntity(const std::shared_ptr<Entity>& parent,const ProjectileProperties& properties, const Point& position, const Vector2d& velocity, const Vector2d& heading)
    : Entity(Entity::Type::Projectile, parent, properties, position, velocity)
    , m_projectileProperties(properties)
{
    m_velocity += heading * (properties.firingForce_N / properties.mass_kg);
}

void ProjectileEntity::update(Engine& engine, uint32_t msElapsed)
{
    m_age += msElapsed;

    if (m_age >= m_projectileProperties.maxAge_ms)
        engine.removeEntity(shared_from_this());

    Entity::update(engine, msElapsed);
}

void ProjectileEntity::collide(Engine& engine, const Entity& otherEntity, const PreCollisionState& otherEntityState)
{
    if (&otherEntity == m_parentEntity.get())
        return;

    engine.removeEntity(shared_from_this());

    ExplosionEntity::ExplosionProperties explosionProps = {};
    explosionProps.radius_km = 1000;

    auto explosionEntity = std::make_shared<ExplosionEntity>(explosionProps, m_position);
    engine.addEntity(explosionEntity);

    Entity::collide(engine, otherEntity, otherEntityState);
}
