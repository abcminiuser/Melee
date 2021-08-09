#include "EntityGenerator.hpp"

using namespace Melee;

EntityGenerator::EntityGenerator(Engine& engine, Entity::Type entityType)
    : m_engine(engine)
    , m_entityType(entityType)
    , m_generatorTimer(0)
{
    m_engine.addObserver(this);

    m_currentTotalEntities = std::count_if(engine.getEntities().begin(), engine.getEntities().end(), [&](const auto& e) { return e->type() == entityType; });
}

EntityGenerator::~EntityGenerator()
{
    m_engine.removeObserver(this);
}

void EntityGenerator::setPeriodicGeneration(uint8_t maxEntities, uint32_t interval)
{
    m_maxEntities = maxEntities;

    m_generatorTimer.setInterval(interval);
    m_generatorTimer.reset();
}

void EntityGenerator::updated(Engine& engine, uint32_t msElapsed)
{
    m_generatorTimer.add(msElapsed);

    if (m_generatorTimer.expired() && m_currentTotalEntities < m_maxEntities)
        generate();
}

void EntityGenerator::entityAdded(Engine& engine, const std::shared_ptr<Entity>& entity)
{
    if (entity->type() == m_entityType)
        m_currentTotalEntities++;
}

void EntityGenerator::entityRemoved(Engine& engine, const std::shared_ptr<Entity>& entity)
{
    if (entity->type() == m_entityType)
        m_currentTotalEntities--;
}
