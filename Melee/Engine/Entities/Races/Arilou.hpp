#pragma once

#include "Engine/Entities/ShipEntity.hpp"
#include "Engine/Entities/WeaponEntity.hpp"

#include "Engine/Util/Periodic.hpp"

namespace Melee
{
    class ArilouShipEntity : public ShipEntity
    {
    public:
        explicit    ArilouShipEntity(const Point& position);
        virtual     ~ArilouShipEntity() = default;

    // ShipEntity i/f:
    protected:
        bool        onPrimaryWeaponFired(Engine& engine) override;
        bool        onSpecialWeaponFired(Engine& engine) override;
        void        onControlFlagsUpdated() override;

    private:
        static ShipEntity::ShipProperties		MakeShipProperties();
    };

    class ArilouLaserWeaponEntity : public WeaponEntity
    {
    public:
        explicit    ArilouLaserWeaponEntity(const std::shared_ptr<Entity>& parent, const Point& position);
        virtual     ~ArilouLaserWeaponEntity() = default;

    private:
        static WeaponEntity::WeaponProperties	MakeWeaponProperties();
    };
}
