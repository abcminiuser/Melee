#pragma once

#include "Engine/Engine.hpp"

namespace Melee
{
    class EntityGenerator : public Engine::Observer
    {
    public:
        explicit                EntityGenerator(Engine& engine, Entity::Type entityType);
        virtual                 ~EntityGenerator();

        void                    setPeriodicGeneration(uint8_t maxEntities, uint32_t interval);

    public:
        virtual void            generate() = 0;

    // Engine::Observer i.f:
    public:
        void                    updated(Engine& engine, uint32_t msElapsed) override;
        void                    entityAdded(Engine& engine, const std::shared_ptr<Entity>& entity) override;
        void                    entityRemoved(Engine& engine, const std::shared_ptr<Entity>& entity) override;

    protected:
        Engine&                 m_engine;
        Entity::Type            m_entityType;

        uint8_t                 m_maxEntities = 0;

        Periodic                m_generatorTimer;
        size_t                  m_currentTotalEntities = 0;
    };
}
