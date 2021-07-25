#include "ProjectileEntity.hpp"

#include "Engine/Engine.hpp"

using namespace Melee;


ProjectileEntity::ProjectileEntity(const ProjectileProperties& properties, const std::shared_ptr<Entity>& ownerEntity, const Point& pos, const Vector2d& heading)
    : Entity(Entity::Type::Projectile, properties, pos, heading)
    , m_ownerEntity(ownerEntity)
    , m_projectileProperties(properties)
{

}

void ProjectileEntity::update(Engine& engine, uint32_t msElapsed)
{
    m_age += msElapsed;

    if (m_age >= m_projectileProperties.maxAge_ms)
        engine.removeEntity(shared_from_this());

    Entity::update(engine, msElapsed);
}

void ProjectileEntity::collide(Engine& engine, const Entity& otherEntity)
{
    if (&otherEntity == m_ownerEntity.get())
        return;

    engine.removeEntity(shared_from_this());

    ExplosionEntity::ExplosionProperties explosionProps = {};
    explosionProps.radius_km = 10;

    auto explosionEntity = std::make_shared<ExplosionEntity>(explosionProps, m_position);
    engine.addEntity(explosionEntity);
}
