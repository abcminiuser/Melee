#include "ExhaustEntity.hpp"

#include "Engine/Engine.hpp"

using namespace Melee;

ExhaustEntity::ExhaustEntity(const std::shared_ptr<Entity>& parent, const ExhaustProperties& properties, const Point& pos, const Vector2d& heading)
    : Entity(Entity::Type::Exhaust, parent, properties, pos, heading)
    , m_exhaustProperties(properties)
{

}

void ExhaustEntity::update(Engine& engine, uint32_t msElapsed)
{
    m_age += msElapsed;

    if (m_age >= m_exhaustProperties.maxAge_ms)
        engine.removeEntity(shared_from_this());

    Entity::update(engine, msElapsed);
}
