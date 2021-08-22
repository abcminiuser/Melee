#pragma once

#include "Engine/Entities/ShipEntity.hpp"
#include "Engine/Entities/WeaponEntity.hpp"

#include "Engine/Util/Periodic.hpp"

namespace Melee
{
    class AndrosynthShipEntity : public ShipEntity
    {
    public:
        explicit    AndrosynthShipEntity(const Point& position);
        virtual     ~AndrosynthShipEntity() = default;

    protected:
        void        update(Engine& engine, uint32_t msElapsed) override;

        bool        onPrimaryWeaponFired(Engine& engine) override;
        bool        onSpecialWeaponFired(Engine& engine) override;

    private:
        void        setBlazerMode(bool enabled);

    private:
        static ShipEntity::ShipProperties		MakeShipProperties();
        static WeaponEntity::WeaponProperties	MakePrimaryWeaponProperties();

    private:
        bool        m_blazerMode = false;
        Periodic    m_blazerEnergyRequired;
    };
}
