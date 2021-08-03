#include "ExplosionEntity.hpp"

#include "Engine/Engine.hpp"
#include "Engine/Util/EngineUtils.hpp"

using namespace Melee;

namespace
{
    constexpr auto kNumSubExplosions = 2;
}

ExplosionEntity::ExplosionEntity(const std::shared_ptr<Entity>& parent, const ExplosionProperties& properties, const Point& position)
    : Entity(Entity::Type::Explosion, parent, properties, position)
    , m_explosionProperties(properties)
{

}

void ExplosionEntity::update(Engine& engine, uint32_t msElapsed)
{
    m_age += msElapsed;

    if (m_age >= m_explosionProperties.maxAge_ms)
        engine.removeEntity(shared_from_this());

    if (!m_subExplosionOccured && NormalizedRandom() < m_explosionProperties.subExplosionProbability)
    {
        m_subExplosionOccured = true;

        ExplosionEntity::ExplosionProperties newProperties = m_explosionProperties;
        newProperties.radius_km /= 2;
        newProperties.maxAge_ms /= 2;
        newProperties.subExplosionProbability /= 2;

        if (newProperties.radius_km && newProperties.maxAge_ms)
        {
            for (int i = 0; i < kNumSubExplosions; i++)
            {
                auto newStartPosition = m_position;
                newStartPosition.x += LinearInterpolateRandom(-m_explosionProperties.radius_km, m_explosionProperties.radius_km);
                newStartPosition.y += LinearInterpolateRandom(-m_explosionProperties.radius_km, m_explosionProperties.radius_km);

                auto explosionEntity = std::make_shared<ExplosionEntity>(shared_from_this(), newProperties, newStartPosition);
                engine.addEntity(explosionEntity);
            }
        }
    }

    Entity::update(engine, msElapsed);
}
