#pragma once

#include "EntityGenerator.hpp"

namespace Melee
{
    class AsteroidGenerator : public EntityGenerator
    {
    public:
        explicit        AsteroidGenerator(Engine& engine);
        virtual         ~AsteroidGenerator() = default;

        void            setVelocityRange(float minVelocity, float maxVelocity);
        void            setRadiusRange(float minRadius, float maxRadius);

    // EntityGenerator i/f:
    public:
        void            generate() override;

    private:
        float           m_minVelocity_km_s = 10;
        float           m_maxVelocity_km_s = 20;

        float           m_minRadius_km = 1000;
        float           m_maxRadius_km = 1500;
    };
}
