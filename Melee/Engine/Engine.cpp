#include "Engine/Engine.hpp"

using namespace Melee;

namespace
{
    constexpr auto kMaxUpdateTimestepMs = 8;
}

Engine::Engine(const uint32_t playfieldSize)
    : m_playfieldSize(playfieldSize)
{

}

Rectangle Engine::getPlayersBoundingBox()
{
    float minX = m_playfieldSize;
    float minY = m_playfieldSize;
    float maxX = 0;
    float maxY = 0;

    for (const auto& entity : m_entitiesForType[Entity::Type::Player])
    {
        const auto pos = entity->position();
        const auto radius = entity->properties().radius_km;

        minX = std::min(minX, pos.x - radius);
        maxX = std::max(maxX, pos.x + radius);
        minY = std::min(minY, pos.y - radius);
        maxY = std::max(maxY, pos.y + radius);
    }

    return { { minX, minY}, {maxX - minX, maxY - minY } };
}

void Engine::update(uint32_t msElapsed)
{
    m_updateMsElapsed += msElapsed;


    while (m_updateMsElapsed >= kMaxUpdateTimestepMs)
    {
        handleDeferredEntityAddRemove();
        checkForEntityCollisions();

        for (const auto& entity : m_entities)
            entity->update(*this, kMaxUpdateTimestepMs);

        m_updateMsElapsed -= kMaxUpdateTimestepMs;
    }
}

void Engine::addEntity(const std::shared_ptr<Entity>& entity) noexcept
{
    m_entitiesToAdd.emplace_back(entity);
}

void Engine::removeEntity(const std::shared_ptr<Entity>& entity) noexcept
{
    m_entitiesToRemove.emplace_back(entity);
}

void Engine::handleDeferredEntityAddRemove() noexcept
{
    if (!m_entitiesToAdd.empty())
    {
        for (const auto& entity : m_entitiesToAdd)
        {
            m_entities.emplace_front(entity);
            m_entitiesForType[entity->type()].emplace_front(entity);
        }

        m_entitiesToAdd.clear();
    }

    if (!m_entitiesToRemove.empty())
    {
        for (const auto& entity : m_entitiesToRemove)
        {
            m_entities.remove(entity);
            m_entitiesForType[entity->type()].remove(entity);
        }

        m_entitiesToRemove.clear();
    }
}

void Engine::checkForEntityCollisions()
{
    const auto totalEntities = m_entities.size();
    if (totalEntities < 2)
        return;

    for (auto e1 = m_entities.begin(); e1 != m_entities.end(); e1++)
    {
        const auto& entity1 = *e1;

        if (!entity1->properties().collidable)
            continue;

        for (auto e2 = std::next(e1); e2 != m_entities.end(); e2++)
        {
            const auto& entity2 = *e2;

            if (!entity2->properties().collidable)
                continue;

            const auto distanceBetweenEntitiesSquared = (entity1->position() - entity2->position()).lengthSquared();
            const auto minCollisionRadius = entity1->properties().radius_km + entity2->properties().radius_km;

            // Crude radius check first, which we can do in the engine without the renderer.
            if (distanceBetweenEntitiesSquared > (minCollisionRadius * minCollisionRadius))
                continue;

            // Ask the renderer to check for a more accurate collision based on the current assets.
            if (m_collisionCallback && !m_collisionCallback(entity1, entity2))
                continue;

            // We need to copy out the pre-collision kinematic states before we start processing the collisions.
            const auto e1Precollision = entity1->preCollisionState();
            const auto e2Precollision = entity2->preCollisionState();

            // Now process the collision on each object.
            entity1->collide(*this, *entity2, e2Precollision);
            entity2->collide(*this, *entity1, e1Precollision);
        }
    }
}
