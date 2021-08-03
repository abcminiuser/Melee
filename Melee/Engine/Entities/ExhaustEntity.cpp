#include "ExhaustEntity.hpp"

#include "Engine/Engine.hpp"

using namespace Melee;

ExhaustEntity::ExhaustEntity(const std::shared_ptr<Entity>& parent, const ExhaustProperties& properties, const Point& position, const Vector2d& velocity)
    : Entity(Entity::Type::Exhaust, parent, properties, position)
    , m_exhaustProperties(properties)
{
    m_velocity = velocity;
}

void ExhaustEntity::update(Engine& engine, uint32_t msElapsed)
{
    m_age += msElapsed;

    if (m_age >= m_exhaustProperties.maxAge_ms)
        engine.removeEntity(shared_from_this());

    Entity::update(engine, msElapsed);
}
