#pragma once

#include "Engine/Entities/ShipEntity.hpp"
#include "Engine/Entities/WeaponEntity.hpp"

namespace Melee
{
    class Race2ShipEntity : public ShipEntity
    {
    public:
        explicit    Race2ShipEntity(const Point& position);
        virtual     ~Race2ShipEntity() = default;

    protected:
        void        onPrimaryWeaponFired(Engine& engine) override;
        void        onSpecialWeaponFired(Engine& engine) override;

    private:
        static ShipEntity::ShipProperties		MakeShipProperties();
        static WeaponEntity::WeaponProperties	MakePrimaryWeaponProperties();
    };
}
