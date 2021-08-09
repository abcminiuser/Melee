#pragma once

#include "EntityGenerator.hpp"

namespace Melee
{
    class PlanetGenerator : public EntityGenerator
    {
    public:
        explicit        PlanetGenerator(Engine& engine);
        virtual         ~PlanetGenerator() = default;

        void            setMassRange(float minMass, float maxMass);
        void            setRadiusRange(float minRadius, float maxRadius);
        void            setMinimumDistanceMultiplier(uint8_t multiplier);

    // EntityGenerator i/f:
    public:
        void            generate() override;

    private:
        float           m_minMass_kg = 3.9736e16f;
        float           m_maxMass_kg = 5.9736e16f;

        float           m_minRadius_km = 4000;
        float           m_maxRadius_km = 8000;

        uint8_t         m_minDistanceMultiplier = 5;
    };
}
