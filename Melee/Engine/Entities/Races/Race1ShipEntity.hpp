#pragma once

#include "Engine/Entities/ShipEntity.hpp"
#include "Engine/Entities/WeaponEntity.hpp"

#include <list>

namespace Melee
{
    class Race1ShipEntity : public ShipEntity
    {
    public:
        explicit    Race1ShipEntity(const Point& position);
        virtual     ~Race1ShipEntity() = default;

    protected:
        void        onPrimaryWeaponFired(Engine& engine) override;
        void        onSpecialWeaponFired(Engine& engine) override;

    private:
        static ShipEntity::ShipProperties		MakeShipProperties();
        static WeaponEntity::WeaponProperties	MakePrimaryWeaponProperties();
        static WeaponEntity::WeaponProperties	MakeSpecialWeaponProperties();

        std::list<std::weak_ptr<WeaponEntity>>  m_mines;
    };
}
