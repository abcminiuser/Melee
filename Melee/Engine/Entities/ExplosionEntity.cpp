#include "ExplosionEntity.hpp"

#include "Engine/Engine.hpp"

using namespace Melee;


ExplosionEntity::ExplosionEntity(const ExplosionProperties& properties, const Point& pos)
    : Entity(Entity::Type::Explosion, properties, pos)
    , m_explosionProperties(properties)
{

}

void ExplosionEntity::update(Engine& engine, uint32_t msElapsed)
{
    m_age += msElapsed;

    if (m_age >= m_explosionProperties.maxAge)
        engine.removeEntity(shared_from_this());

    Entity::update(engine, msElapsed);
}
