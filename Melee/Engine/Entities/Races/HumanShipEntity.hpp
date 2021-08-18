#pragma once

#include "Engine/Entities/ShipEntity.hpp"
#include "Engine/Entities/WeaponEntity.hpp"

namespace Melee
{
    class HumanShipEntity : public ShipEntity
    {
    public:
        explicit    HumanShipEntity(const Point& position);
        virtual     ~HumanShipEntity() = default;

    protected:
        void        onEngineExhaustGenerated(Engine& engine) override;
        void        onPrimaryWeaponFired(Engine& engine) override;
        void        onSpecialWeaponFired(Engine& engine) override;

    private:
        static ShipEntity::ShipProperties		MakeShipProperties();
        static WeaponEntity::WeaponProperties	MakePrimaryWeaponProperties();
        static WeaponEntity::WeaponProperties	MakeSpecialWeaponProperties();
    };
}
