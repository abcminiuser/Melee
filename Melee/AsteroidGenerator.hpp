#pragma once

#include "Engine/Engine.hpp"

namespace Melee
{
    class AsteroidGenerator : public Engine::Observer
    {
    public:
        explicit    AsteroidGenerator(Engine& engine, uint32_t generationIntervalMs);
        virtual     ~AsteroidGenerator();

        void        setMaxAsteroids(uint8_t maxAsteroids);
        void        setVelocityRange(float minVelocity, float maxVelocity);
        void        setRadiusRange(float minRadius, float maxRadius);

    // Engine::Observer i.f:
    public:
        void        updated(Engine& engine, uint32_t msElapsed) override;
        void        entityAdded(Engine& engine, const std::shared_ptr<Entity>& entity) override;
        void        entityRemoved(Engine& engine, const std::shared_ptr<Entity>& entity) override;

    private:
        Engine&     m_engine;

        uint8_t     m_maxAsteroids = 1;

        float       m_minVelocity_km_s = 10;
        float       m_maxVelocity_km_s = 20;

        float       m_minRadius_km = 1000;
        float       m_maxRadius_km = 1500;

        Periodic    m_generatorTimer;
        size_t      m_currentTotalAsteroids = 0;
    };
}
