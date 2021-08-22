#pragma once

#include "Engine/Entity.hpp"

namespace Melee
{
    class PlanetEntity : public Entity
    {
    public:
        enum class VisualType
        {
			Acid,
			Alkali,
			Auric,
			Azure,
			Bluegas,
			Carbide,
			Chlorine,
			Chondrite,
			Cimmerian,
			Copper,
			Crimson,
			Cyangas,
			Cyanic,
			Dust,
			Emerald,
			Fluorescent,
			Green,
			Greengas,
			Greygas,
			Halide,
			Hydrocarbon,
			Infrared,
			Iodine,
			Lanthanide,
			Magma,
			Magnetic,
			Maroon,
			Metal,
			Noble,
			Oolite,
			Opalescent,
			Organic,
			Pellucid,
			Plutonic,
			Primordial,
			Purple,
			Purplegas,
			Quasidegenerate,
			Radioactive,
			Rainbow,
			Redgas,
			Redux,
			Ruby,
			Sapphire,
			Selenic,
			Shattered,
			Slaveshield,
			Superdense,
			Telluric,
			Treasure,
			Ultramarine,
			Urea,
			Vinylogous,
			Violetgas,
			Water,
			Xenolithic,
			Yellowgas,
			Yttric,
        };

        struct PlanetProperties : public Properties
        {
			PlanetProperties()
			{
				collidable = true;
				affectedByGravity = false,
				collisionDamage = 1;
			}

            VisualType visualType = VisualType::Acid;
        };

    public:
        explicit        PlanetEntity(const PlanetProperties& properties, const Point& position);
        virtual         ~PlanetEntity() = default;

        VisualType      visualType() const  { return m_visualType; }

    // Entity i/f:
    public:
        void            update(Engine& engine, uint32_t msElapsed) override;

    private:
        VisualType      m_visualType = VisualType::Acid;
    };
}
